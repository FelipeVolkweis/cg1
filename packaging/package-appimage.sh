#!/bin/sh

set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BINARY="$ROOT_DIR/${1:-build/cg}"
OUTPUT="$ROOT_DIR/${2:-bin/cg.AppImage}"
APPIMAGE_TOOL="$ROOT_DIR/${3:-bin/appimagetool-x86_64.AppImage}"
APPDIR="$ROOT_DIR/tmp/cg.AppDir"

rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin" "$APPDIR/usr/lib" "$APPDIR/usr/share/cg"

cp "$BINARY" "$APPDIR/usr/bin/cg"
cp -R "$ROOT_DIR/data" "$ROOT_DIR/shaders" "$APPDIR/usr/share/cg/"
cp "$ROOT_DIR/packaging/cg.desktop" "$APPDIR/cg.desktop"
cp "$ROOT_DIR/packaging/cg.png" "$APPDIR/cg.png"

copy_library() {
    library_prefix=$1
    library_entry=$(ldd "$BINARY" | awk -v prefix="$library_prefix" \
        'index($1, prefix) == 1 { print $1 "|" $3; exit }')
    library_name=${library_entry%%|*}
    library_path=${library_entry#*|}

    if [ -z "$library_entry" ] || [ ! -f "$library_path" ]; then
        echo "Could not locate required library matching: $library_prefix" >&2
        exit 1
    fi

    cp -L "$library_path" "$APPDIR/usr/lib/$library_name"
}

for library in \
    libglfw.so. \
    libBulletDynamics.so. \
    libBulletCollision.so. \
    libLinearMath.so. \
    libBulletSoftBody.so. \
    libyaml-cpp.so. \
    libtinyobjloader.so. \
    libstdc++.so. \
    libgcc_s.so.
do
    copy_library "$library"
done

cat > "$APPDIR/AppRun" <<'EOF'
#!/bin/sh
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
export LD_LIBRARY_PATH="$HERE/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
cd "$HERE/usr/share/cg"
exec "$HERE/usr/bin/cg" "$@"
EOF

chmod +x "$APPDIR/AppRun"

if command -v strip >/dev/null 2>&1; then
    strip --strip-unneeded "$APPDIR/usr/bin/cg" "$APPDIR"/usr/lib/*.so* || true
fi

mkdir -p "$(dirname -- "$OUTPUT")"
rm -f "$OUTPUT"
ARCH=x86_64 "$APPIMAGE_TOOL" --appimage-extract-and-run "$APPDIR" "$OUTPUT"
chmod +x "$OUTPUT"

echo "Created $OUTPUT"
