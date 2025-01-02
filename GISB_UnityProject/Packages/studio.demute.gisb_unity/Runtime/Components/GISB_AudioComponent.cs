using System.Collections;
using System.Collections.Generic;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;


namespace GISB.Runtime
{
    public class GISB_AudioComponent : MonoBehaviour
    {
        public GISB_Event associatedEvent;
        public Dictionary<string, string> activeParameters = new Dictionary<string, string>();
        
        public Dictionary<GISB_Event, GISB_EventInstance> activeEventInstances = new Dictionary<GISB_Event, GISB_EventInstance>();

        public void PlayEvent()
        {
            PlayEvent(associatedEvent);
        }
        
        public GISB_EventInstance PlayEvent(GISB_Event eventToPlay)
        {
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

    }
}

