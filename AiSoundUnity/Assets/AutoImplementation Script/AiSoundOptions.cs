using Newtonsoft.Json;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class AiSoundOptions : ScriptableObject
{
    public static string jsonSavePath = Path.Combine("Assets", "JsonExports");

    public static string parseSavePath = Path.Combine("Assets", "ParseResults");

    public static string rootDirectory = Path.Combine("Assets", "AiSoundEvents");

    public static string eventRootDirectory = Path.Combine(rootDirectory, "SoundEvents");

    public static string bankRootDirectory = Path.Combine(rootDirectory, "SoundBanks");

    
}
