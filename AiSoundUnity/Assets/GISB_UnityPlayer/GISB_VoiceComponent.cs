using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GISB_VoiceComponent : MonoBehaviour
{
    private AudioSource _audioSource;
    private SoundEvent _soundEvent;
    
    public void PlaySound(SoundEvent soundEvent, AudioSource audioSource, Dictionary<string, object> currentParameters)
    {
        _audioSource = audioSource;
        _soundEvent = soundEvent;
        soundEvent.rootSoundContainer.PlaySound(audioSource, currentParameters);
    }

    public void UpdateParameter(Dictionary<string, object> updatedParameters)
    {
        _soundEvent.rootSoundContainer.UpdateParameter(_audioSource, updatedParameters);
    }
}
