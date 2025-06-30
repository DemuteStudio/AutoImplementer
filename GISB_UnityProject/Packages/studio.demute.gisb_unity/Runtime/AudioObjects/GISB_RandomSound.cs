using System.Collections;
using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [SRName("Random Sound")]
    public class GISB_RandomSound : GISB_AudioObjectBase
    {
        public int avoidLastPlayed = 0;
        
        public bool loop = false;

        public enum TransitionMode
        {
            TriggerRate,
            Crossfade
        }
        
        public TransitionMode transitionMode = TransitionMode.TriggerRate;

        public float triggerRate = 1.0f;
        public float crossfadeDuration = 1.0f;
        
        [SerializeReference, GISB_AudioList]
        public GISB_AudioObjectBase[] RandomPlaylist;
        
        public override GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null)
        {
            return new GISB_RandomSoundPlayer(this, parent);
        }

        public override Dictionary<string, List<string>> ExtractParameters()
        {
            Dictionary<string, List<string>> parameters = new Dictionary<string, List<string>>();
            foreach (GISB_AudioObjectBase audioObject in RandomPlaylist)
            {
                if(audioObject == null)
                {
                    continue;
                }
                Dictionary<string, List<string>> extractedParameters = audioObject.ExtractParameters();
                foreach (KeyValuePair<string, List<string>> extractedParameter in extractedParameters)
                {
                    if (!parameters.ContainsKey(extractedParameter.Key))
                    {
                        parameters.Add(extractedParameter.Key, new List<string>());
                    }
                    parameters[extractedParameter.Key].AddRange(extractedParameter.Value);
                }
            }
            return parameters;
        }

        public override float ExtractMaxDistance()
        {
            float maxDistance = attenuation.overrideParent && attenuation.value != null && attenuation.value.active ? attenuation.value.maxDistance : 0f;
            foreach (GISB_AudioObjectBase audioObject in RandomPlaylist)
            {
                if(audioObject == null)
                {
                    continue;
                }
                float objectMaxDistance = audioObject.ExtractMaxDistance();
                if (objectMaxDistance > maxDistance)
                {
                    maxDistance = objectMaxDistance;
                }
            }
            return maxDistance;
        }
    }
}
