using Newtonsoft.Json;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class JsonValidation
{
    [MenuItem("Assets/AiSound/Validate Json", true)]
    private static bool ValidateValidateJson()
    {
        foreach (Object obj in Selection.objects)
        {
            string path = AssetDatabase.GetAssetPath(obj);
            if (! path.EndsWith(".json"))
                return false;
        }
        return true;
    }

    [MenuItem("Assets/AiSound/Validate Json")]
    private static void ValidateJson()
    {
        JsonConvert.DefaultSettings = () => new JsonSerializerSettings  // Sets the JsonConverter to the custom made one
        {
            Converters = { new AiSoundConverter() }
        };

        foreach (Object obj in Selection.objects)
        {
            string json = File.ReadAllText(AssetDatabase.GetAssetPath(obj));
            AiSoundEvent activeEvent = JsonConvert.DeserializeObject<AiSoundEvent>(json);
            activeEvent.DebugDisplay();
        }       
    }
}
