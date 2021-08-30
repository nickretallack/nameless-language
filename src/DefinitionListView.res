@react.component
let make = (~definitions: DefinitionMap.t, ~languageName: LanguageName.t) =>
  <table>
    <thead>
      <tr>
        <th> {"Kind"->React.string} </th>
        <th> {"Name"->React.string} </th>
        <th> {"Type"->React.string} </th>
      </tr>
    </thead>
    <tbody>
      {RenderList.f(
        ListSortBy.f(Belt.Map.String.toList(definitions), ((_, definition: Definition.t)) =>
          ListFindIndexExn.f(
            ImplementationType.list,
            ImplementationToImplementationType.f(definition.implementation),
          )
        ),
        (_, (definitionID: DefinitionID.t, definition: Definition.t)) =>
          <tr key=definitionID>
            <td> {ImplementationGetName.f(definition.implementation)->React.string} </td>
            <td>
              <a href={`#${definitionID}/implementation`}>
                {DefinitionGetDisplayName.f(definition, languageName)->React.string}
              </a>
            </td>
          </tr>,
      )}
    </tbody>
  </table>
