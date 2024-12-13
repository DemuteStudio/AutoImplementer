using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

[System.Serializable]
[CreateAssetMenu(fileName = "New RandomSound", menuName = "AiSound/RandomSound")]
public class RandomSound : AiSoundContainer
{
    [SerializeField]
    public AiSoundContainer[] soundArray;

    [SerializeField]
    public float[] soundWeights = null;

    public override void PlaySound(AudioSource audioSource, Dictionary<string, object> parameterList)
    {
        AiSoundContainer soundContainer = null;

        bool weightCheck = soundWeights != null && soundWeights.Length != soundArray.Length;

        if (weightCheck)
        {
            float randomNumber = UnityEngine.Random.Range(0f, 100f);

            float weightIncrementation = 0;
            for (int i = 0; i < soundWeights.Length; i++)
            {
                weightIncrementation += soundWeights[i];
                if (weightIncrementation >= randomNumber)
                {
                    soundContainer = soundArray[i];
                    break;
                }
            }
        }
        else
        {
            int randomNumber = UnityEngine.Random.Range(0, soundArray.Length);
            soundContainer = soundArray[randomNumber];
        }
        
        if (soundContainer != null) soundContainer.PlaySound(audioSource, parameterList);
        else throw new FileNotFoundException("No sound file");
    }

    public override void CopySoundFiles(string jsonSavePath)
    {
        foreach (AiSoundContainer container in soundArray)
        {
            if (container != null)
            {
                container.CopySoundFiles(jsonSavePath);
            }
        }
    }

    public override void ImportSoundFiles(string importDirectory)
    {
        foreach (AiSoundContainer container in soundArray)
        {
            if (container != null)
            {
                container.ImportSoundFiles(importDirectory);
            }
        }
    }

    public override void DebugDisplay()
    {
        string appendedString = "AiAudio From Json Display : \n\nRandomSound";

        for (int i = 0; i < soundArray.Length; i++)
        {
            appendedString += soundArray[i].ReturnDisplay();
            appendedString += $"\t Chance : {soundWeights[i]}%";
        }

        Debug.Log(appendedString);
    }
    public override string ReturnDisplay(string formatting)
    {
        string appendedString = formatting + "RandomSound";

        for (int i = 0; i < soundArray.Length; i++)
        {
            appendedString += soundArray[i].ReturnDisplay(formatting + "\t");
            appendedString += $"\t Chance : {soundWeights[i]}%";
        }

        return appendedString;
    }
    public override void UnpackAiSound(string rootDirectory, string passedName)
    {
        string path = AssetDatabase.GenerateUniqueAssetPath(Path.Combine(rootDirectory, "Rdm_" + passedName));

        if (!Directory.Exists(path))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(path);
            AssetDatabase.SaveAssets();
        }

        foreach (AiSoundContainer container in soundArray)
        {
            if (container != null)
            {
                container.UnpackAiSound(path, passedName);
            }
        }

        AssetDatabase.CreateAsset(this, AssetDatabase.GenerateUniqueAssetPath(Path.Combine(path, "Rdm_" + passedName + ".asset")));
        AssetDatabase.SaveAssets();
    }
}