// AiSound.js
// Tests for the Auto Implementing tool, trying to create every basic kind of container in Fmod through scripting
// Dev : Rob3
// Version : 0.01

// --- Defining menu items --- //
studio.menu.addMenuItem({
    name: "Create Switch Sound",
    isEnabled: true,
    execute: createSwitchSound,
});
studio.menu.addMenuItem({
    name: "Create Random Sound",
    isEnabled: true,
    execute: createRandomSound,
});
studio.menu.addMenuItem({
    name: "Create Simple Sound",
    isEnabled: true,
    execute: createSimpleSound,
});

// --- Main test functions --- //

function createSwitchSound() {
    // --- Creating the event --- //
    const eventSwitch = newEvent("SwitchSound");
    
    // --- Creating the parameter sheet --- //
    const param = eventSwitch.addGameParameter({
        name: "ParameterTest",
        type: studio.project.parameterType.UserEnumeration,
    });
    
    param.preset.isGlobal = true;
    const paramArr = ["label1", "label2","label3","label4","label5","label6"];
    param.preset.enumerationLabels = paramArr;
    param.preset.maximum = paramArr.length; //Allows for the parameter sheet to display correctly in GUI


    // --- EventSound tests --- //
    const track = eventSwitch.masterTrack;
    const instrument = track.addSound(param, "EventSound", 1, 1);
        
    //param.properties.dump();
}

function createRandomSound(){
    
    // --- Creating the event --- //
    const eventRandom = newEvent("RandomSound");

    // --- Creating the Randomizing Action Sheet --- //
    const actionSheet = newActionSheet(eventRandom);
    
    const rdmInst = studio.project.create('MultiSound');
    rdmInst.owner = actionSheet.modules;
    
    // --- EventSound tests --- //
    const etest = studio.project.create("Event");
    etest.name = "Nested Event";
    etest.folder = eventRandom;
    
    const eventN1 = studio.project.create("EventSound");
    eventN1.event = etest;
    eventN1.owner = rdmInst;
}

function createSimpleSound(){
    // --- Creating the event --- //
    const eventSimple = newEvent("SimpleSound");
    
    // --- Creating the Randomizing Action Sheet --- //
    const actionSheet = newActionSheet(eventSimple);

    const sglInst = studio.project.create('SingleSound');
    sglInst.owner = actionSheet.modules;
}

// --- Secondary test functions --- //
function newEvent(name){
    const event = studio.project.create("Event");
    event.name = name;
    event.timeline.isProxyEnabled = false;    //Removes the default timeline sheet
    
    return event;
}

function newActionSheet(event){
    const actionSheet = studio.project.create("ActionSheet");
    const actModules = studio.project.create('MultiSound');
    actionSheet.modules = actModules;
    event.relationships.parameters.add(actionSheet);
    actModules.audioTrack = event.masterTrack;
    
    return actionSheet;
}