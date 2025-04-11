using System;
using System.Collections;
using System.Collections.Generic;
using PlasticPipe.PlasticProtocol.Messages;
using UnityEngine;

namespace GISB.Runtime
{
    [System.Serializable]
    public class GISB_Attenuation
    {
        public enum AttenuationPreset
        {
            Linear,
            Logarithmic,
            Inverse,
            Custom
        }
        
        public bool active = false;
        public float minDistance = 1;
        public float maxDistance = 10;
        public AttenuationPreset preset = AttenuationPreset.Linear;
        public AnimationCurve curve = AnimationCurve.Linear(0, 1, 1, 0);
        public float volumeAtMaxDistance = 0.0f;
        public float spreadAtMinDistance = 0.0f;
        public float spreadAtMaxDistance = 0.0f;
        public float lowPassAtMinDistance = 0.0f;
        public float lowPassAtMaxDistance = 0.0f;
        //public float highPassAtMinDistance = 0.0f;
        //public float highPassAtMaxDistance = 0.0f;

        public static AnimationCurve GetAttenuationCurveForPreset(AttenuationPreset preset)
        {
            switch (preset)
            {
                case AttenuationPreset.Linear:
                    return AnimationCurve.Linear(0, 1, 1, 0);

                case AttenuationPreset.Logarithmic:
                    AnimationCurve logcurve = new AnimationCurve();
                    logcurve.AddKey(new Keyframe(0, 1, 0, -2.302f));
                    logcurve.AddKey(new Keyframe(1, 0, -0.105f, 0));
                    return logcurve;

                case AttenuationPreset.Inverse:
                    AnimationCurve invcurve = new AnimationCurve();
                    invcurve.AddKey(new Keyframe(0, 1, 0, -3f));
                    invcurve.AddKey(new Keyframe(1, 0, 0, 0));
                    return invcurve;

                case AttenuationPreset.Custom:
                    return null;

                default:
                    throw new ArgumentOutOfRangeException(nameof(preset), preset, null);
            }
        }
    }
    
   
}