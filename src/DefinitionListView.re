[@react.component]
let make = (~definitions: DefinitionMap.t) => {
  <ul>
    {RenderStringMap.f(
       ((definitionID, definition)) =>
         <li key=definitionID>
           <a href={"#" ++ definitionID}>
             {ReasonReact.string(DefinitionGetDisplayName.f(definition, "en"))}
           </a>
         </li>,
       definitions,
     )}
  </ul>;
};
