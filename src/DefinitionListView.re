[@react.component]
let make = (~definitions: DefinitionMap.t) => {
  <table>
    <thead>
      <tr>
        <th> "Kind"->ReasonReact.string </th>
        <th> "Name"->ReasonReact.string </th>
        <th> "Type"->ReasonReact.string </th>
      </tr>
    </thead>
    <tbody>
      {RenderList.f(
         ListSortBy.f(
           Belt.Map.String.toList(definitions),
           ((_, definition: Definition.t)) =>
           ListFindIndexExn.f(
             ImplementationType.list,
             ImplementationToImplementationType.f(definition.implementation),
           )
         ),
         (_, (definitionID: DefinitionID.t, definition: Definition.t)) =>
         <tr key=definitionID>
           <td>
             {ImplementationGetName.f(definition.implementation)
              ->ReasonReact.string}
           </td>
           <td>
             <a href={"#" ++ definitionID}>
               {DefinitionGetDisplayName.f(definition, "en")
                ->ReasonReact.string}
             </a>
           </td>
         </tr>
       )}
    </tbody>
  </table>;
};
