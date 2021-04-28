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
  ~stackFrame: option<MaterializedStackFrame.t>,
  ~urlHashRest: array<string>,
) => {
  let (state, dispatch) = ReactUpdate.useReducer(
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
    GraphReducer.f(emit, implementation),
  )
  <div id="graph-view">
    <nav id="graph-nav" className="nav-buttons">
      <a
        href={`#${definitionID}/documentation`}
        className={urlHashRest == ["documentation"] ? "active" : ""}>
        {React.string("Documentation")}
      </a>
      <a href={`#${definitionID}`} className={urlHashRest == [] ? "active" : ""}>
        {React.string("Implementation")}
      </a>
      {switch state.selection {
      | SelectedNib(explicitConnectionSide) => <>
          <a href={`#${definitionID}/+node`} className={urlHashRest == ["+node"] ? "active" : ""}>
            {React.string("+Node")}
          </a>
          <button onClick={_event => emit(EvaluateNib(explicitConnectionSide))}>
            {React.string("Debug")}
          </button>
        </>
      | SelectedConnection(connectionSide) => <>
          <button onClick={_event => emit(RemoveConnection(connectionSide))}>
            {React.string("Remove connection")}
          </button>
          <button
            onClick={_event =>
              emit(EvaluateNib({connectionSide: connectionSide, isSource: false}))}>
            {React.string("Debug")}
          </button>
        </>
      | SelectedNodes(_) =>
        <button onClick={_event => dispatch(RemoveSelectedNodes)}>
          {React.string("Remove Node(s)")}
        </button>
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
    {switch urlHashRest {
    | ["documentation"] =>
      <GraphDocumentationView
        definitionID definitions implementation display documentation languageName emit error
      />
    | ["+node"] =>
      <NodeAddView
        emit
        graphDefinitionID=definitionID
        definitions
        languageName
        nodes=implementation.nodes
        selection=state.selection
      />
    | [] =>
      <GraphImplementationView
        definitions implementation definition display languageName state stackFrame emit=dispatch
      />
    | _ => React.string("Not found")
    }}
  </div>
}
