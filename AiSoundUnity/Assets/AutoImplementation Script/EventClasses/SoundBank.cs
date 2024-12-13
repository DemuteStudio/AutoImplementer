using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using UnityEditor;
using UnityEngine;


[System.Serializable]
[CreateAssetMenu(fileName = "New SoundBank", menuName = "AiSound/Events/SoundBank")]
public class SoundBank : AiSoundEvent
{

    [SerializeField]
    public EventDictionary soundBankEvents;

    public override void DebugDisplay()
    {
        string appendedString = "AiAudio From Json Display : \n\nBank: " + eventName;

        foreach (KeyValuePair<string, SoundEvent> entry in soundBankEvents)
        {
            appendedString += "\n\t" + entry.Key;

            if (entry.Value != null)
                appendedString += entry.Value.ReturnDisplay("\n\t\t");
            else
                appendedString += "\n\t\tNULL";
        }
        Debug.Log(appendedString);
    }
    public override string ReturnDisplay(string formatting)
    {
        string appendedString = formatting + "Bank: "+ eventName;

        foreach (KeyValuePair<string, SoundEvent> entry in soundBankEvents)
        {
            appendedString += formatting + "\t" + entry.Key;

            if (entry.Value != null)
                appendedString += entry.Value.ReturnDisplay(formatting + "\t");
            else
                appendedString += formatting + "\tNULL";
        }
        return appendedString;
    }

    public override void UnpackAiSound(string rootDirectory, string passedName)
    {
        string path = AssetDatabase.GenerateUniqueAssetPath(Path.Combine(rootDirectory, "Bank_" + eventName));

        if (!Directory.Exists(path))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(path);
            AssetDatabase.SaveAssets();
        }

        foreach (KeyValuePair<string, SoundEvent> entry in soundBankEvents)
        {
            if (entry.Value != null)
            {
                entry.Value.UnpackAiSound(path, eventName);
            }
        }

        AssetDatabase.CreateAsset(this, AssetDatabase.GenerateUniqueAssetPath(Path.Combine(path, "Bank_" + eventName + ".asset")));
        AssetDatabase.SaveAssets();
    }

    public override void CopySoundFiles(string jsonSavePath)
    {
        foreach (KeyValuePair<string, SoundEvent> entry in soundBankEvents)
        {
            if (entry.Value != null)
            {
                entry.Value.CopySoundFiles(jsonSavePath);
            }
        }
    }

    public override void ImportSoundFiles(string importDirectory)
    {
        foreach (KeyValuePair<string, SoundEvent> entry in soundBankEvents)
        {
            if (entry.Value != null)
            {
                entry.Value.ImportSoundFiles(importDirectory);
            }
        }
    }

    public bool GetSoundEvent(string eventNameString, out SoundEvent soundEvent)
    {
        return soundBankEvents.TryGetValue(eventNameString, out soundEvent);
    }
}