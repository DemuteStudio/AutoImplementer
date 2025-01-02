using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEngine;

public class GISB_EventInstance : MonoBehaviour
{
    public GISB_Event eventDefinition;
    private GISB_AudioObjectPlayer rootPlayerInstance;
    private List<AudioSource> audioSources = new List<AudioSource>();

    public void Play(Dictionary<string, string> activeParameters)
    {
        if(rootPlayerInstance == null)
        {
            rootPlayerInstance = eventDefinition.GetPlayer();
        }
        
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.Play(activeParameters, this);
        }
    }

    public AudioSource GetAudioVoice()
    {
        foreach (AudioSource audioSource in audioSources)
        {
            if (!audioSource.isPlaying)
            {
                return audioSource;
            }
        }
        
        GameObject newAudioSourceObject = new GameObject("Audio Voice");
        newAudioSourceObject.transform.parent = this.transform;
        AudioSource newAudioSource = newAudioSourceObject.AddComponent<AudioSource>();
        audioSources.Add(newAudioSource);
        return newAudioSource;
    }
}
