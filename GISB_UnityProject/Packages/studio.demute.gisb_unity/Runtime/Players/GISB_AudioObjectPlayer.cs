using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public abstract class GISB_AudioObjectPlayer
    {
        public abstract void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance);

        public abstract void UpdateParameters(Dictionary<string, string> activeParameters);
    }
}

