using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using GISB.Runtime;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;
using UnityEngine.Networking;

namespace GISB.Editor
{
    [CustomEditor(typeof(GISB_Bank))]
    public class GISB_BankEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            base.OnInspectorGUI();
            
            GISB_Bank bank = (GISB_Bank) target;

            if (GUILayout.Button("Add new event"))
            {
                string bankPath = Path.GetDirectoryName(AssetDatabase.GetAssetPath(bank));
                string path = EditorUtility.SaveFilePanelInProject("Save GISB Event", 
                    "New Event", "asset", "Save new GISB Event", bankPath);
                if (path.Length != 0)
                {
                    GISB_Event newEvent = ScriptableObject.CreateInstance<GISB_Event>();
                    AssetDatabase.CreateAsset(newEvent, path);
                    bank.events.Add(newEvent);
                    EditorUtility.SetDirty(bank);
                }
            }
            
            if (GUILayout.Button("Export to JSON"))
            {
                string path = EditorUtility.SaveFilePanel("Save GISB Bank", Application.dataPath, target.name, "json");
                if (path.Length != 0)
                {
                    string folderPath = Path.GetDirectoryName(path);
                    JsonSerializerSettings settings = new JsonSerializerSettings
                    {
                        TypeNameHandling = TypeNameHandling.Auto,
                        Converters = new List<JsonConverter> { new GISB_EventRefJsonConverter(folderPath, false) }
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
                    ImportFromJSON(path, bank, false);
                }
            }

            if (GUILayout.Button("Download from server"))
            {
                string eventPath = GISB_EditorSettings.LoadOrDefault().JSONImportAddress + bank.name + ".json";
                ImportFromJSON(eventPath, bank, true);
            }
        }

        public static void ImportFromJSON(string path, GISB_Bank bank, bool webUrl)
        {
            string folderPath = Path.GetDirectoryName(path);
            JsonSerializerSettings settings = new JsonSerializerSettings
            {
                TypeNameHandling = TypeNameHandling.Auto,
                Converters = new List<JsonConverter> { new GISB_EventRefJsonConverter(folderPath, webUrl) }
            };
            string json = "";
            if (webUrl)
            {
                using (UnityWebRequest webRequest = UnityWebRequest.Get(path))
                {
                    webRequest.SendWebRequest();
                    while (!webRequest.isDone)
                    {
                        EditorUtility.DisplayProgressBar("Downloading", path, webRequest.downloadProgress);
                    }
                    EditorUtility.ClearProgressBar();
                    if (webRequest.error != null)
                    {
                        Debug.LogError(webRequest.error);
                        return;
                    }
                    json = webRequest.downloadHandler.text;
                }
            }
            else
            {
                json = System.IO.File.ReadAllText(path);
            }
            JsonConvert.PopulateObject(json, bank, settings);
            EditorUtility.SetDirty(bank);
            //Auto save asset
            AssetDatabase.SaveAssetIfDirty(bank);
        }
    }
}
