using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

[System.Serializable]
public abstract class AiSoundEvent : AiSound
{
    [SerializeField]
    public string eventName;
}
