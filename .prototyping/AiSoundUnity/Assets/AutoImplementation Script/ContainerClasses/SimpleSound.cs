using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

[System.Serializable]
[CreateAssetMenu(fileName = "New SimpleSound", menuName = "AiSound/SimpleSound")]
public class SimpleSound : AiSoundContainer
{
    [SerializeField, HideInInspector]
    public string soundPath;
    [JsonIgnore]
    public AudioClip soundFile;

    public override void PlaySound(AudioSource audioSource, Dictionary<string, object> parameterList)
    {
        if (soundFile != null)
        {
            audioSource.clip = soundFile;
            audioSource.Play();
        }

        else throw new FileNotFoundException("No sound file");
    }

    public override void CopySoundFiles(string jsonSavePath)
    {
        //Copy the audio file to the jsonSavePath
        string filename = Path.GetFileName(AssetDatabase.GetAssetPath(soundFile));
        //Create directory
        if (!Directory.Exists(jsonSavePath + "/Audio Files/"))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(jsonSavePath + "/Audio Files/");
        }
        File.Copy(AssetDatabase.GetAssetPath(soundFile), Path.Combine(jsonSavePath+"/Audio Files/", filename), true);
    }

    public override void ImportSoundFiles(string importDirectory)
    {
        //Import the audio file from the importDirectory
        
    }

    public override void DebugDisplay()
    {
        Debug.Log("SimpleSound : " + soundFile.name);
    }
    public override string ReturnDisplay(string formatting)
    {
        return formatting + "SimpleSound : " + soundFile.name;
    }
    public override void UnpackAiSound(string rootDirectory, string passedName)
    {
        //TODO : get a way to go back to the root directory of the import to get the audio files
        
        string filename = Path.GetFileNameWithoutExtension(soundFile.name);

        string path = AssetDatabase.GenerateUniqueAssetPath(Path.Combine(rootDirectory, "Smpl_" + passedName + "_" + filename));

        if (!Directory.Exists(path))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(path);
            AssetDatabase.SaveAssets();
        }
        

        AssetDatabase.CreateAsset(this, AssetDatabase.GenerateUniqueAssetPath(Path.Combine(path, "Smpl_" + passedName + "_" + filename + ".asset")));
        AssetDatabase.SaveAssets();
    }
}