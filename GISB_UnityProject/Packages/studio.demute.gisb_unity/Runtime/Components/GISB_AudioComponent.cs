using System.Collections;
using System.Collections.Generic;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;
using UnityEngine.Serialization;


namespace GISB.Runtime
{
    public class GISB_AudioComponent : MonoBehaviour
    {
        [FormerlySerializedAs("associatedPlayable")] public GISB_Asset associatedAsset;
        public Dictionary<string, string> activeParameters = new Dictionary<string, string>();
        
        public Dictionary<GISB_Event, GISB_EventInstance> activeEventInstances = new Dictionary<GISB_Event, GISB_EventInstance>();

        public GISB_EventInstance PlayEvent()
        {
            return PlayEvent(associatedAsset.GetEvent(""));
        }
        
        public GISB_EventInstance PlayEvent(GISB_Event eventToPlay)
        {
            if(!eventToPlay) return null;

            if(!activeEventInstances.ContainsKey(eventToPlay) || activeEventInstances[eventToPlay] == null)
            {
                GameObject newEventInstanceObject = new GameObject(eventToPlay.name);
                newEventInstanceObject.transform.parent = this.transform;
                GISB_EventInstance newEventInstance = newEventInstanceObject.AddComponent<GISB_EventInstance>();
                newEventInstance.eventDefinition = eventToPlay;
                activeEventInstances[eventToPlay] = newEventInstance;
            }

            activeEventInstances[eventToPlay].Play(activeParameters);
            return activeEventInstances[eventToPlay];
        }
        
        public GISB_EventInstance PlayEvent(string eventName)
        {
            GISB_Event eventToPlay = associatedAsset.GetEvent(eventName);
            return PlayEvent(eventToPlay);
        }

    }
}

