#pragma once

namespace Space
{
    class AssetManager
    {
    public:
        AssetManager(std::string assetDir)
        {
            this->m_AssetDir = assetDir;
        }

        static std::string GetAssetDirectory() { return m_AssetDir; }

        // Get the absolute path to a file (e.g. "C:/Users/someUser/Downloads")
        // @param relativePath
        // The path to the file from the root directory
        static std::string GetAbsolutePath(std::string relativePath) { return m_AssetDir + relativePath; }
    private:
        inline static std::string m_AssetDir;
    };
}
