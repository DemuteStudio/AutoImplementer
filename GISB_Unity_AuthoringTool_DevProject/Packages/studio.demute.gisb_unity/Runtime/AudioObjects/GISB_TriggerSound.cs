using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;
using UnityEngine.Serialization;

namespace GISB.Runtime
{
    [SRName("Trigger Sound")]
    public class GISB_TriggerSound : GISB_AudioObjectBase
    {
        [Tooltip("Number of times to trigger the sound. -1 for infinite.")]
        public int triggerAmount = 1;
        public enum TransitionMode
        {
            TriggerRate,
            Crossfade
        }
        
        public TransitionMode transitionMode = TransitionMode.TriggerRate;

        public float triggerRate = 1.0f;
        public float crossfadeDuration = 1.0f;

        [SerializeReference, SR]
        public GISB_AudioObjectBase TriggeredSound;
        
        public override GISB_BaseAudioPlayer GetPlayer(GISB_BaseAudioPlayer parent = null)
        {
            return new GISB_TriggerSoundPlayer(this, parent);
        }

        public override Dictionary<string, List<string>> ExtractParameters()
        {
            if(TriggeredSound == null)
            {
                return new Dictionary<string, List<string>>();
            }
            return TriggeredSound.ExtractParameters();
        }

        public override List<string> ExtractFloatParameters()
        {
            List<string> floatParameters = new List<string>();
            if(TriggeredSound != null)
            {
                floatParameters = TriggeredSound.ExtractFloatParameters();
            }

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
            float maxDistance = attenuation.overrideParent && attenuation.value != null && attenuation.value.active ? attenuation.value.maxDistance : 0f;
            if(TriggeredSound != null && TriggeredSound.ExtractMaxDistance() > maxDistance)
            {
                maxDistance = TriggeredSound.ExtractMaxDistance();
            }
            return maxDistance;
        }
    }
}