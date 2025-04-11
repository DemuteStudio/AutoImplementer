using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    [System.Serializable]
    public class GISB_VolumeParameter : GISB_RandomizableParameter<float>
    {
        public GISB_VolumeParameter()
        {
            randomize = false;
            minModifier = 0.0f;
            maxModifier = 0.0f;
            baseValue = 0.0f;
        }
        
        public override float GetRandomValue()
        {
            if (randomize)
            {
                return Random.Range(baseValue+minModifier, baseValue+maxModifier);
            }
            else
            {
                return baseValue;
            }
        }
    }
}

