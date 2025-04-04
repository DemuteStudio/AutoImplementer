using System.Collections;
using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [SRName("Blend Sound")]
    public class GISB_BlendSound : GISB_AudioObjectBase
    {
        [SerializeReference, SR]
        public GISB_AudioObjectBase[] BlendPlaylist;
        
        public override GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null)
        {
            return new GISB_BlendSoundPlayer(this, parent);
        }
        
        public override Dictionary<string, List<string>> ExtractParameters()
        {
            Dictionary<string, List<string>> parameters = new Dictionary<string, List<string>>();
            foreach (GISB_AudioObjectBase audioObject in BlendPlaylist)
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
            float maxDistance = attenuation.overrideParent && attenuation.value.active ? attenuation.value.maxDistance : 0f;
            foreach (GISB_AudioObjectBase audioObject in BlendPlaylist)
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

