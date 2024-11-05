using System.Collections.Generic;
using UnityEngine;


[System.Serializable]
public abstract class AiSoundContainer : AiSound
{
    public abstract void PlaySound(AudioSource audioSource, Dictionary<string, object> parameterList = null); //audioSource is where to play the sound ; parameterList is a list of parameters ;

}