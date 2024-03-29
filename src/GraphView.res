@react.component
let make = (
  ~definitionID: DefinitionID.t,
  ~definitions: DefinitionMap.t,
  ~implementation: GraphImplementation.t,
  ~definition: Definition.t,
  ~display: DefinitionDisplay.t,
  ~documentation: Documentation.t,
  ~languageName: LanguageName.t,
  ~emit: DefinitionAction.t => unit,
  ~error: AppError.t,
  ~subNav: string,
  ~execution: option<Execution.t>,
  ~scopeID: option<ScopeID.t>,
) => {
  let (state, dispatch) = ReactUpdate.useReducer(
    GraphReducer.f(emit, implementation),
    {
      open GraphState
      {
        pointers: Belt.Map.make(~id=module(PointerComparable.C)),
        error: None,
        selection: NoSelection,
        scroll: {x: 0.0, y: 0.0},
        zoom: 1.0,
      }
    },
  )

  <div id="graph-view">
    <nav id="graph-nav" className="nav-buttons">
      <a
        href={`#${definitionID}/documentation`}
        className={subNav == "documentation" ? "active" : ""}>
        {React.string("Documentation")}
      </a>
      <a
        href={`#${definitionID}/implementation`}
        className={subNav == "implementation" ? "active" : ""}>
        {React.string("Implementation")}
      </a>
      <button onClick={_ => emit(Publish)}> {React.string("Publish")} </button>
      {switch state.selection {
      | SelectedNib(explicitConnectionSide) => <>
          <a href={`#${definitionID}/+node`} className={subNav == "+node" ? "active" : ""}>
            {React.string("+Node")}
          </a>
          <button
            onClick={_event =>
              emit(EvaluateNib({explicitConnectionSide: explicitConnectionSide, debug: true}))}>
            {React.string("Debug")}
          </button>
          <button
            onClick={_event =>
              emit(EvaluateNib({explicitConnectionSide: explicitConnectionSide, debug: false}))}>
            {React.string("Run")}
          </button>
        </>
      | SelectedConnection(connectionSide) => <>
          <button onClick={_event => emit(RemoveConnection(connectionSide))}>
            {React.string("Remove connection")}
          </button>
          <button
            onClick={_event =>
              emit(
                EvaluateNib({
                  explicitConnectionSide: {connectionSide: connectionSide, isSource: false},
                  debug: true,
                }),
              )}>
            {React.string("Debug")}
          </button>
          <button
            onClick={_event =>
              emit(
                EvaluateNib({
                  explicitConnectionSide: {connectionSide: connectionSide, isSource: false},
                  debug: false,
                }),
              )}>
            {React.string("Run")}
          </button>
        </>
      | SelectedNodes(nodeIDs) =>
        let (_, _, scopes) = GraphNodesGetScopes.f(implementation, nodeIDs)

        <>
          <button onClick={_event => dispatch(RemoveSelectedNodes)}>
            {React.string("Remove Node(s)")}
          </button>
          {if Belt.Set.size(scopes) == 1 {
            <button onClick={_event => dispatch(NewFunctionFromSelection)}>
              {React.string("Make Function")}
            </button>
          } else {
            React.null
          }}
        </>
      | NoSelection => React.null
      }}
      {switch state.error {
      | Some(error) => <div className="error-message"> {React.string(error)} </div>
      | None => React.null
      }}
      {switch error {
      | NoAppError | NibIsConnected(_) => React.null
      | ConnectionCrossesScopeError =>
        <div>
          {React.string(
            "When crossing scopes, you can only connect a source in a parent scope to a sink in a child scope.  You may have to remove some connections in order to change the scope of this node.",
          )}
        </div>
      }}
    </nav>
    {switch subNav {
    | "documentation" =>
      <GraphDocumentationView
        definitionID definitions implementation display documentation languageName emit error
      />
    | "+node" =>
      <NodeAddView
        emit
        graphDefinitionID=definitionID
        definitions
        languageName
        nodes=implementation.nodes
        selection=state.selection
      />
    | "implementation" =>
      <GraphImplementationView
        definitions
        implementation
        definition
        display
        languageName
        state
        execution
        scopeID
        emit=dispatch
      />
    | _ => React.string("Not found")
    }}
  </div>
}
