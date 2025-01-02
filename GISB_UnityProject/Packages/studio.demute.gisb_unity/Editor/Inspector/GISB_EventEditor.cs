using System.Collections;
using System.Collections.Generic;
using System.IO;
using GISB.Runtime;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;

namespace GISB.Editor
{
    [CustomEditor(typeof(GISB_Event))]
    public class GISB_EventEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
        
            if(GUILayout.Button("Export to JSON"))
            {
                string path = EditorUtility.SaveFilePanel("Save GISB Event", Application.dataPath, target.name, "json");
                if (path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    GISB_Event gisbEvent = (GISB_Event)target;
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_AudioClipJsonConverter(folderPath) }
                    };
                    string json = JsonConvert.SerializeObject(gisbEvent, Formatting.Indented, settings);
                    System.IO.File.WriteAllText(path, json);
                }
            }

            if (GUILayout.Button("Import from JSON"))
            {
                string path = EditorUtility.OpenFilePanel("Import GISB Event", Application.dataPath, "json");
                if(path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_AudioClipJsonConverter(folderPath) }
                    };
                    string json = System.IO.File.ReadAllText(path);
                    JsonConvert.PopulateObject(json, target, settings);
                    EditorUtility.SetDirty(target);
                    //Auto save asset
                    AssetDatabase.SaveAssetIfDirty(target);
                }
            }
        }
    }
}

