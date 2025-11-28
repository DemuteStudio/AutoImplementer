using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;

namespace GISB.Editor
{
    public class GISB_EditorSettings : ScriptableObject
{
    public const string DefaultGeneratedDataPath = "GISB";
    public const string DefaultResourcesPath = "Assets/GISB/Configuration";
    public List<string> foundJSONFiles = new List<string>();
    
    [Tooltip("Relative (to the application data directory) path to store the automatically generated assets.")]
    public string generatedDataPath = DefaultGeneratedDataPath;

    public string JSONExportAddress = "file://";
    public string JSONImportAddress = "file://";
    
    private static string GetGeneratedDataPath()
    {
        string localPath = LoadOrDefault().generatedDataPath;
        string path = Path.Combine(Application.dataPath, localPath);
        if (!Directory.Exists(path)) Directory.CreateDirectory(path);
        return path;
    }

    public static GISB_EditorSettings LoadOrDefault()
    {
        string settingsPath = $"{DefaultResourcesPath}/GISB_EditorSettings.asset";
        GISB_EditorSettings settings = AssetDatabase.LoadAssetAtPath<GISB_EditorSettings>(settingsPath);
        if (!settings)
        {
            settings = CreateInstance<GISB_EditorSettings>();
            if (!Directory.Exists(DefaultResourcesPath)) Directory.CreateDirectory(DefaultResourcesPath);
            AssetDatabase.CreateAsset(settings, settingsPath);
            AssetDatabase.SaveAssets();
        }
        return settings;
    }
    
    internal static SerializedObject GetSerializedSettings()
    {
        return new SerializedObject(LoadOrDefault());
    }
}

public class GISB_EditorSettingsProvider : SettingsProvider
{
    private SerializedObject m_SerializedObject;
    
    public GISB_EditorSettingsProvider(string path, SettingsScope scopes = SettingsScope.Project) : base(path, scopes)
    {
    }
    
    public override void OnActivate(string searchContext, VisualElement rootElement)
    {
        m_SerializedObject = GISB_EditorSettings.GetSerializedSettings();
    }

    public override void OnGUI(string searchContext)
    {
        m_SerializedObject.Update();
        //Show all the properties in the serializedObject
        SerializedProperty property = m_SerializedObject.GetIterator();
        property.NextVisible(true);
        property.NextVisible(true);
        do
        {
            EditorGUILayout.PropertyField(property, true);
        } while (property.NextVisible(false));
        
        m_SerializedObject.ApplyModifiedProperties();
    }

    [SettingsProvider]
    public static SettingsProvider CreateSettingsProvider()
    {
        return new GISB_EditorSettingsProvider("Project/GISB Unity", SettingsScope.Project);
    }
}
}

