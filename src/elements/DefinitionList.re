[%%debugger.chrome];
open Helpers;
open Definition;

let component = ReasonReact.statelessComponent("DefinitionList");

let make = (~definitions: definitions, _children) => {
  ...component,
  render: _self =>
    <ul>
      {renderStringMap(
         ((definitionID, definition)) =>
           <li key=definitionID>
             <a href={"#" ++ definitionID}>
               {ReasonReact.string(getDisplayName(definition, "en"))}
             </a>
           </li>,
         definitions,
       )}
    </ul>,
};
