- Action sheets are actually just big multi-instruments
	- to be setup correctly you need to create a multi-instrument to act as ActionSheet.modules container
```js
const actionSheet = studio.project.create("ActionSheet");  
const actModules = studio.project.create('MultiSound');  
actionSheet.modules = actModules;  
eventRandom.relationships.parameters.add(actionSheet);  
actModules.audioTrack = eventRandom.masterTrack;
```
