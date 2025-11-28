using System;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    [SRName("Single Sound")]
    public class GISB_SingleSound : GISB_AudioObjectBase
    {
        public AudioClip soundClip;
        public bool loop = false;
        
        public override GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null)
        {
            return new GISB_SingleSoundPlayer(this, parent);
        }

        public override Dictionary<string, List<string>> ExtractParameters()
        {
            return new Dictionary<string, List<string>>();
        }
        
        public override List<string> ExtractFloatParameters()
        {
            List<string> floatParameters = new List<string>();
            foreach (GISB_Modulator modulator in modulators)
            {
                if (!floatParameters.Contains(modulator.ParameterName))
                {
                    floatParameters.Add(modulator.ParameterName);
                }            
            }
            return floatParameters;
        }

        public override float ExtractMaxDistance()
        {
            return attenuation.overrideParent && attenuation.value != null && attenuation.value.active ? attenuation.value.maxDistance : 0f;
        }
    }
}

