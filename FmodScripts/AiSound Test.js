// AiSound Test.js
// This script creates a test event and lists the declared function in the Scripts tab

studio.menu.addMenuItem({
    name: "AiSound Test",
    isEnabled: true,
    execute: createEventWithnestedEvents,
});

function createEventWithnestedEvents() {
    const mainEvent = studio.project.create("Event");
    mainEvent.name = "v16";
    var event = mainEvent;

    for (var i = 0; i < 4; i++) {
        const track = event.masterTrack;
        const instrument = track.addSound(event.timeline, "EventSound", 0, 10);
        
        event = instrument.event;
    }
    
    //instrument.relationships.dump();
    
}





