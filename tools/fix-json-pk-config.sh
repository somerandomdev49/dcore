# IDK what this does, help
echo "will maybe work"

echo 'export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$(dirname $(brew ls nlohmann-json | grep ".pc"))' >> ~/.profile
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$(dirname $(brew ls nlohmann-json | grep ".pc"))
