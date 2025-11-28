using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

[System.Serializable]
[CreateAssetMenu(fileName = "New SoundEvent", menuName = "AiSound/Events/SoundEvent")]
public class SoundEvent : AiSoundEvent
{

    [SerializeField]
    public AiSoundContainer rootSoundContainer;

    public override void DebugDisplay()
    {
        if(rootSoundContainer != null)
            Debug.Log("AiAudio From Json Display : \n\nEvent: " + eventName + rootSoundContainer.ReturnDisplay());
        else
            Debug.Log("AiAudio From Json Display : \n\nEvent: " + eventName + "\n\tNULL");
    }
    public override string ReturnDisplay(string formatting)
    {
        if (rootSoundContainer != null)
            return formatting + "Event: " + eventName + rootSoundContainer.ReturnDisplay(formatting + "\t");

        return formatting + "Event: " + eventName + formatting + "\tNULL";
    }
    public override void UnpackAiSound(string rootDirectory, string passedName)
    {
        string path = AssetDatabase.GenerateUniqueAssetPath(Path.Combine(rootDirectory, "Event_" + eventName));

        if (!Directory.Exists(path))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(path);
            AssetDatabase.SaveAssets();
        }

        if (rootSoundContainer != null)
        {
            rootSoundContainer.UnpackAiSound(path, eventName);
        }

        AssetDatabase.CreateAsset(this, AssetDatabase.GenerateUniqueAssetPath(Path.Combine(path, "Event_" + eventName + ".asset")));
        AssetDatabase.SaveAssets();
    }

    public override void CopySoundFiles(string jsonSavePath)
    {
        if (rootSoundContainer != null)
        {
            rootSoundContainer.CopySoundFiles(jsonSavePath);
        }
    }

    public override void ImportSoundFiles(string importDirectory)
    {
        if (rootSoundContainer != null)
        {
            rootSoundContainer.ImportSoundFiles(importDirectory);
        }
    }
}