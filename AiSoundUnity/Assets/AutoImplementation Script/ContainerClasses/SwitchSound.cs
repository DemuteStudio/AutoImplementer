using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;

[System.Serializable]
[CreateAssetMenu(fileName = "New SwitchSound", menuName = "AiSound/SwitchSound")]
public class SwitchSound : AiSoundContainer
{
    [SerializeField]
    public string parameterID;
    [SerializeField]
    public ContainerDictionary soundDictionary;

    public override void PlaySound(AudioSource audioSource, Dictionary<string, object> parameterList)
    {
        object parameter;

        if (!parameterList.TryGetValue(parameterID, out parameter))
        {
            parameter = soundDictionary.Keys.First();
        }

        AiSoundContainer soundContainer = soundDictionary[(string)parameter];

        if (soundContainer != null) soundContainer.PlaySound(audioSource, parameterList);

        else throw new FileNotFoundException("No sound file");
    }

    public override void CopySoundFiles(string jsonSavePath)
    {
        foreach (KeyValuePair<string, AiSoundContainer> entry in soundDictionary)
        {
            if (entry.Value != null)
            {
                entry.Value.CopySoundFiles(jsonSavePath);
            }
        }
    }

    public override void ImportSoundFiles(string importDirectory)
    {
        foreach (KeyValuePair<string, AiSoundContainer> entry in soundDictionary)
        {
            if (entry.Value != null)
            {
                entry.Value.ImportSoundFiles(importDirectory);
            }
        }
    }

    public override void DebugDisplay()
    {
        string appendedString = "AiAudio From Json Display : \n\nSwitchSound (" + parameterID + ")";

        foreach (KeyValuePair<string, AiSoundContainer> entry in soundDictionary)
        {
            appendedString += "\n\t" + entry.Key;
            appendedString += entry.Value.ReturnDisplay("\n\t\t");
        }
        Debug.Log(appendedString);
    }
    public override string ReturnDisplay(string formatting)
    {
        string appendedString = formatting + "SwitchSound (" + parameterID + ")";

        foreach (KeyValuePair<string, AiSoundContainer> entry in soundDictionary)
        {
            appendedString += formatting + "\t" + entry.Key;
            appendedString += entry.Value.ReturnDisplay(formatting + "\t\t");
        }
        return appendedString;
    }
    public override void UnpackAiSound(string rootDirectory, string passedName)
    {
        string path = AssetDatabase.GenerateUniqueAssetPath(Path.Combine(rootDirectory, "Switch_" + passedName + "_" + parameterID));

        if (!Directory.Exists(path))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(path);
            AssetDatabase.SaveAssets();
        }

        foreach (KeyValuePair<string, AiSoundContainer> entry in soundDictionary)
        {
            if (entry.Value != null)
            {
                entry.Value.UnpackAiSound(path, entry.Key);
            }
        }

        AssetDatabase.CreateAsset(this, AssetDatabase.GenerateUniqueAssetPath(Path.Combine(path, "Switch_" + passedName + "_" + parameterID + ".asset")));
        AssetDatabase.SaveAssets();
    }
}