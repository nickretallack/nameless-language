let f = (publishingDependency: PublishingDependency.t): Js.Json.t => {
  open Json.Encode
  switch publishingDependency.kind {
    | Final(hashedContent) => HashedContentToJson.f(hashedContent)
  }
}