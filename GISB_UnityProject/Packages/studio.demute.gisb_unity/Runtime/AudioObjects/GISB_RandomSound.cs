using System.Collections;
using System.Collections.Generic;
using SerializeReferenceEditor;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_RandomSound : GISB_AudioObjectBase
    {
        public int avoidLastPlayed = 0;
        
        [SerializeReference, SR]
        public GISB_AudioObjectBase[] RandomPlaylist;
        
        public override GISB_AudioObjectPlayer GetPlayer()
        {
            return new GISB_RandomSoundPlayer(this);
        }
    }
}
