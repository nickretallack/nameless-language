let f = (kind: PublishingDependencyKind.t) =>
  switch kind {
  | Final({contentID}) => contentID
  | Recursion => "self"
  | UnorderedMutualRecursion => "mutual"
  | OrderedMutualRecursion(position) => `mutual-${string_of_int(position)}`
  | FinalMutualRecursion({position, content: {contentID}}) =>
    `${contentID}.${string_of_int(position)}`
  }
