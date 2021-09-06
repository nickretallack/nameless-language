@react.component
let make = (~definitions: DefinitionMap.t, ~languageName: LanguageName.t) =>
  <div id="definition-list">
    <table>
      <thead>
        <tr> <th> {"Name"->React.string} </th> <th> {"Type"->React.string} </th> </tr>
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
              <td>
                <a href={`#${definitionID}/implementation`}>
                  {DefinitionGetDisplayName.f(definition, languageName)->React.string}
                </a>
              </td>
              <td> {ImplementationGetName.f(definition.implementation)->React.string} </td>
            </tr>,
        )}
      </tbody>
    </table>
  </div>
