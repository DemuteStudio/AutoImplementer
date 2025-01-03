using System.Collections;
using System.Collections.Generic;
using System.IO;
using GISB.Runtime;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;

namespace GISB.Editor
{
    [CustomEditor(typeof(GISB_Bank))]
    public class GISB_BankEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
            
            GISB_Bank bank = (GISB_Bank) target;

            if (GUILayout.Button("Export to JSON"))
            {
                string path = EditorUtility.SaveFilePanel("Save GISB Bank", Application.dataPath, target.name, "json");
                if (path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_EventRefJsonConverter(folderPath) }
                    };
                    string json = JsonConvert.SerializeObject(bank, Formatting.Indented, settings);
                    System.IO.File.WriteAllText(path, json);
                }
            }

            if (GUILayout.Button("Import from JSON"))
            {
                string path = EditorUtility.OpenFilePanel("Import GISB Bank", Application.dataPath, "json");
                if (path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_EventRefJsonConverter(folderPath) }
                    };
                    string json = System.IO.File.ReadAllText(path);
                    JsonConvert.PopulateObject(json, bank, settings);
                    EditorUtility.SetDirty(bank);
                    //Auto save asset
                    AssetDatabase.SaveAssetIfDirty(bank);
                }
            }
        }
    }
}
