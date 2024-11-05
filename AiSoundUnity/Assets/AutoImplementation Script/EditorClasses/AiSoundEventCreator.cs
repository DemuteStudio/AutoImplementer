using UnityEngine;
using UnityEditor;
using System.IO;

[ExecuteInEditMode]
public class AiSoundEventCreator : EditorWindow
{
    public enum AssetType
    {
        SoundEvent,
        SoundBank
    }

    private AssetType selectedAssetType;
    private string assetName = "New Asset";
    private string directoryPath = null;

    [MenuItem("Tools/AiSound/Event Creator")]
    public static void ShowWindow()
    {
        GetWindow<AiSoundEventCreator>();
    }

    private void OnGUI()
    {
        selectedAssetType = (AssetType)EditorGUILayout.EnumPopup("Select Asset Type", selectedAssetType);
        assetName = EditorGUILayout.TextField("Asset Name", assetName);

        if (GUILayout.Button("Create Asset"))
        {
            CreateAsset();
        }
    }

    public void CreateAsset()
    {
        AiSoundEvent newAsset = null;

        switch (selectedAssetType)
        {
            case AssetType.SoundEvent:
                newAsset = CreateInstance<SoundEvent>();
                directoryPath = AiSoundOptions.eventRootDirectory;
                break;
            case AssetType.SoundBank:
                newAsset = CreateInstance<SoundBank>();
                directoryPath = AiSoundOptions.bankRootDirectory;
                break;
        }

        if (newAsset != null)
        {
            newAsset.eventName = assetName;

            string assetPath = SetPath();

            AssetDatabase.CreateAsset(newAsset, assetPath);
            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();

            Close();
            EditorUtility.FocusProjectWindow();
            Selection.activeObject = newAsset;
        }
    }

    public string SetPath()     //Makes sure the necessary folders are created and returns asset path
    {

        if (!Directory.Exists(AiSoundOptions.rootDirectory))   //Creates root directory if not already
        {
            Directory.CreateDirectory(AiSoundOptions.rootDirectory);
        }

        if (!Directory.Exists(directoryPath))   //Creates event/bank root directory exists
        {
            Directory.CreateDirectory(directoryPath);
        }

        AssetDatabase.SaveAssets();
        AssetDatabase.Refresh();

        return AssetDatabase.GenerateUniqueAssetPath(Path.Combine(directoryPath, assetName+".asset"));
    }

}