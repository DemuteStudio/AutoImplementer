using Newtonsoft.Json;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;

public class AiSoundJsonParser
{
    [MenuItem("Assets/AiSound/Parse Json to Object", true)]
    private static bool ValidateJsonParser()    //Checks if selected menu items are Jsons
    {
        foreach (Object obj in Selection.objects)
        {
            string path = AssetDatabase.GetAssetPath(obj);
            if (!path.EndsWith(".json"))
                return false;
        }
        return true;
    }

    [MenuItem("Assets/AiSound/Parse Json to Object")]
    private static void JsonParser()
    {
        JsonConvert.DefaultSettings = () => new JsonSerializerSettings  // Sets the JsonConverter to the custom made one
        {
            Converters = { new AiSoundConverter() }
        };

        foreach (Object obj in Selection.objects)
        {
            string json = File.ReadAllText(AssetDatabase.GetAssetPath(obj));
            AiSoundEvent activeEvent = JsonConvert.DeserializeObject<AiSoundEvent>(json);
            CreateAiSound(activeEvent);
            activeEvent.DebugDisplay();
        }
    }

    private static void CreateAiSound(AiSoundEvent soundEvent)
    {
        if (!Directory.Exists(AiSoundOptions.parseSavePath))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(AiSoundOptions.parseSavePath);
            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }

        if (soundEvent != null)     //Saves asset to project if not null
        {

            soundEvent.UnpackAiSound(AiSoundOptions.parseSavePath);

            EditorUtility.FocusProjectWindow();
            Selection.activeObject = soundEvent;

            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }

    }
}