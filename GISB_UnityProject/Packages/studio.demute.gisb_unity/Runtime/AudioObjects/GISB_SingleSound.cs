using System;
using System.Collections;
using System.Collections.Generic;
using Newtonsoft.Json;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SingleSound : GISB_AudioObjectBase
    {
        public AudioClip soundClip;
        
        public override GISB_AudioObjectPlayer GetPlayer()
        {
            return new GISB_SingleSoundPlayer(this);
        }
    }
}

