using System.Collections;
using System.Collections.Generic;
using System.IO;
using GISB.Runtime;
using Newtonsoft.Json;
using UnityEditor;
using UnityEngine;
using UnityEngine.Networking;

namespace GISB.Editor
{
    public class GISB_BankBrowser : EditorWindow
    {
        public List<string> banks = new List<string>();
        
        private SerializedProperty outputProperty;
        
        [MenuItem("Window/GISB/Bank Browser")]
        public static void ShowWindow()
        {
            GISB_BankBrowser window = (GISB_BankBrowser)EditorWindow.GetWindow(typeof(GISB_BankBrowser));
            window.titleContent = new GUIContent("GISB Bank Browser");
            window.Show();
        }
        
        public void ChooseEvent(SerializedProperty property)
        {
            outputProperty = property;
        }
        

        private void OnGUI()
        {
            if (GUILayout.Button("Reload Banks"))
            {
                ReloadBanks();
            }

            foreach (string bankPath in banks)
            {
                if (GUILayout.Button(bankPath))
                {
                    string eventAssetsFolder = "Assets/GISB/Banks/";
                    if (!Directory.Exists(eventAssetsFolder))
                    {
                        Directory.CreateDirectory(eventAssetsFolder);
                    }
                    GISB_Bank bankAsset = AssetDatabase.LoadAssetAtPath<GISB_Bank>(eventAssetsFolder + bankPath + ".asset");
                    if (bankAsset == null)
                    {
                        bankAsset = ScriptableObject.CreateInstance<GISB_Bank>();
                        AssetDatabase.CreateAsset(bankAsset, eventAssetsFolder + bankPath + ".asset");
                    }
                    string bankURL = GISB_EditorSettings.LoadOrDefault().JSONImportAddress + bankPath + ".json";
                    GISB_BankEditor.ImportFromJSON(bankURL, bankAsset, true);
                    AssetDatabase.SaveAssetIfDirty(bankAsset);
                    if(outputProperty != null)
                    {
                        outputProperty.objectReferenceValue = bankAsset;
                        outputProperty.serializedObject.ApplyModifiedProperties();
                    }
                    Close();
                }
            }
        }
        
        private void ReloadBanks()
        {
            banks.Clear();
            string bankListingUrl = GISB_EditorSettings.LoadOrDefault().JSONImportAddress + "BankList.json";
            
            // Download bank list
            using (UnityWebRequest www = UnityWebRequest.Get(bankListingUrl))
            {
                www.SendWebRequest();
                while (!www.isDone)
                {
                    EditorUtility.DisplayProgressBar("Downloading bank list", www.downloadProgress.ToString(), www.downloadProgress);
                }
                EditorUtility.ClearProgressBar();

                if (www.error != null)
                {
                    Debug.LogError(www.error);
                }
                else
                {
                    string json = www.downloadHandler.text;
                    JsonConvert.PopulateObject(json, this);
                }
            }
        }
    }
}



