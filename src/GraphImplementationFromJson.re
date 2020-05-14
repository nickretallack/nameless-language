let f = (json: Js.Json.t): GraphImplementation.t => Json.Decode.(
	GraphImplementation.{
		nodes: field("nodes", MapFromJson.f(NodeFromJson.f, json) json),
		// connections: field("connections", )
	}
)
