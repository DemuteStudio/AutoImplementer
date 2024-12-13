using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GISB_PlayerComponent : MonoBehaviour
{
    //TODO : interface to choose this one from a list
    public SoundBank selectedSoundBank;
    
    private Dictionary<string, object> _currentParameters = new Dictionary<string, object>();
    
    private List<GISB_VoiceComponent> _playingVoices = new List<GISB_VoiceComponent>();

    //TODO : Maybe return the instance of the sound was played
    public void PlaySound(string eventName)
    {
        if (selectedSoundBank == null) return;
        
        if(selectedSoundBank.GetSoundEvent(eventName, out SoundEvent soundEvent))
        {
            //We have found event, play it
            //Create empty child gameobject
            //Rename it
            //Add AudioSource component
            //Add GISB_VoiceComponent component
            //Play sound
            
            GameObject spawnedVoice = new GameObject();
            spawnedVoice.transform.parent = transform;
            spawnedVoice.name = eventName;
            AudioSource audioSource = spawnedVoice.AddComponent<AudioSource>();
            GISB_VoiceComponent voiceComponent = spawnedVoice.AddComponent<GISB_VoiceComponent>();
            voiceComponent.PlaySound(soundEvent, audioSource, _currentParameters);
            _playingVoices.Add(voiceComponent);
        }
    }
    
    public void StopSound(string eventName)
    {
        
    }

    public void StopAllSounds()
    {
        
    }
    
    public void SetParameter(string parameterName, string parameterValue)
    {
        _currentParameters[parameterName] = parameterValue;
        
        foreach (GISB_VoiceComponent voiceComponent in _playingVoices)
        {
            voiceComponent.UpdateParameter(_currentParameters);
        }
    }
    
    public void OnVoiceEnded(GISB_VoiceComponent voiceComponent)
    {
        _playingVoices.Remove(voiceComponent);
        Destroy(voiceComponent.gameObject);
    }
}
