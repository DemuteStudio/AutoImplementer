using Newtonsoft.Json;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;

public class AiSoundSerializer
{

    [MenuItem("Assets/AiSound/Serialize to Json", validate = true)]
    private static bool ValidateSerializeSound()
    {
        // Only display the context menu item if an AiSound asset is selected
        foreach (Object obj in Selection.objects)
        {
            if (obj is not AiSoundEvent)
                return false;
        }
        return true;
    }

    [MenuItem("Assets/AiSound/Serialize to Json")]
    private static void SerializeSound()
    {
        JsonConvert.DefaultSettings = () => new JsonSerializerSettings  // Sets the JsonConverter to the custom made one
        {
            Converters = { new AiSoundConverter() }
        };

        if (!Directory.Exists(AiSoundOptions.jsonSavePath))   //Creates root directory if not already
        {
            Directory.CreateDirectory(AiSoundOptions.jsonSavePath);
        }

        foreach (AiSoundEvent soundEvent in Selection.objects.Cast<AiSoundEvent>())
        {
            string json = JsonConvert.SerializeObject(soundEvent, Formatting.Indented);

            string directory = AiSoundOptions.jsonSavePath + "/" + soundEvent.eventName + "/";
            if(Directory.Exists(directory))
            {
                Directory.Delete(directory, true);
            }
            Directory.CreateDirectory(directory);

            string filePath = Path.Combine(directory, soundEvent.eventName + ".json");
            File.WriteAllText(filePath, json);
            
            //Copy all the sound files to the json save path
            soundEvent.CopySoundFiles(directory);
        }
        AssetDatabase.SaveAssets();
        AssetDatabase.Refresh();
    }
}
