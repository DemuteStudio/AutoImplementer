using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SingleSoundPlayer : GISB_AudioObjectPlayer
    {
        public GISB_SingleSound audioObject;
        
        public GISB_SingleSoundPlayer(GISB_SingleSound audioObject)
        {
            this.audioObject = audioObject;
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance)
        {
            AudioSource audioSource = gisbEventInstance.GetAudioVoice();
            
            audioSource.clip = audioObject.soundClip;
            audioSource.Play();
        }
    }
}
