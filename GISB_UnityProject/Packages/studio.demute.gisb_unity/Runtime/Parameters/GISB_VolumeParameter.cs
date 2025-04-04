using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    [System.Serializable]
    public class GISB_VolumeParameter : GISB_RandomizableParameter<float>
    {
        public override float GetRandomValue()
        {
            if (randomize)
            {
                return Random.Range(baseValue+minModifieer, baseValue+maxModifier);
            }
            else
            {
                return baseValue;
            }
        }
    }
}

