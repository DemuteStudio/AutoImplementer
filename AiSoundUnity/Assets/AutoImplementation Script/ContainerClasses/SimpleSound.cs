using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

[System.Serializable]
[CreateAssetMenu(fileName = "New SimpleSound", menuName = "AiSound/SimpleSound")]
public class SimpleSound : AiSoundContainer
{
    [SerializeField]
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
    public override void DebugDisplay()
    {
        Debug.Log("SimpleSound : " + soundPath);
    }
    public override string ReturnDisplay(string formatting)
    {
        return formatting + "SimpleSound : " + soundPath;
    }
    public override void UnpackAiSound(string rootDirectory, string passedName)
    {
        string filename = Path.GetFileNameWithoutExtension(soundPath);

        string path = AssetDatabase.GenerateUniqueAssetPath(Path.Combine(rootDirectory, "Smpl_" + passedName + "_" + filename));

        if (!Directory.Exists(path))   //Creates root directory if it doesn't exist
        {
            Directory.CreateDirectory(path);
            AssetDatabase.SaveAssets();
        }

        // THIS IS WHERE YOU DOWNLOAD THE AUDIO FILE

        AssetDatabase.CreateAsset(this, AssetDatabase.GenerateUniqueAssetPath(Path.Combine(path, "Smpl_" + passedName + "_" + filename + ".asset")));
        AssetDatabase.SaveAssets();
    }
}