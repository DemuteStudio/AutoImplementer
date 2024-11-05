using Newtonsoft.Json;
using System.IO;
using UnityEngine;

[System.Serializable]
public abstract class AiSound : ScriptableObject
{
    public abstract void DebugDisplay();
    public abstract string ReturnDisplay(string formatting = "\n\t");
    public abstract void UnpackAiSound(string rootDirectory, string passedName = "");
}