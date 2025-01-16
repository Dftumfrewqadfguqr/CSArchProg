mkdir -p ./deb_http_client
mkdir -p ./deb_http_client/bin
mkdir -p ./deb_http_client/DEBIAN
touch  ./deb_http_client/DEBIAN/control

cat << EOF > ./deb_http_client/DEBIAN/control
Package: HTTPClient
Version: 1.0
Architecture: amd64
Maintainer: Dftumfrewqadfguqr
EOF

cp ./build/HTTPClient ./deb_http_client/bin/

dpkg-deb --build ./deb_http_client/ HTTPClient.deb
