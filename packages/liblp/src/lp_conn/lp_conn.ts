import { LpNodeDef } from "src/lp_node/lp_node";
import { LpScopeDef } from "src/lp_scope/lp_scope";
import { typedPath } from "typed-path";
import { lp_obj } from "../lp_obj/lp_obj";

export interface lp_conn {
  sourceNodeId: string | null
  sourcePath: LpObjectPath
  destPath: LpObjectPath
}

export type lp_conn_def = lp_obj<"connection_def"> & lp_conn
export type lp_conn_instance = lp_obj<"connection_instance"> & lp_conn

type LpObjectPath = Array<string | number>
type LpPathBuilderResult = { $rawPath: Array<string | symbol | number> }

export function lpObjectPathFrom(result: LpPathBuilderResult): LpObjectPath {
  return result.$rawPath.flatMap(it => typeof it === 'symbol' ? [] : [ it ])
}

// ---------------------------------------------------------------------------------------------------------------------

export function scopeInput<
  TScope extends LpScopeDef
>(
  // @ts-ignore
  scope: TScope,
  sourcePath: ScopeInputPathBuilder<TScope>
) {
  return {
    scope, sourcePath
  }
}

export type ScopeInputPathBuilder<TScope extends LpScopeDef> = (p: ReturnType<typeof typedPath<TScope["_input"]>>) => { $rawPath: Array<string | symbol | number> }

export interface ScopeInput<TScope extends LpScopeDef = LpScopeDef> {
  scope: TScope,
  inputPath: LpObjectPath

  sendTo(dest: NodeInput<TScope>): void
}

export function isScopeInput(value: any): value is ScopeInput {
  return ("scope" satisfies keyof ScopeInput) in value
    && ("inputPath" satisfies keyof ScopeInput) in value;
}

// ---------------------------------------------------------------------------------------------------------------------
export type ScopeOutputPathBuilder<TScope extends LpScopeDef> = (p: ReturnType<typeof typedPath<TScope["_output"]>>) => { $rawPath: Array<string | symbol | number> }

export interface ScopeOutput<TScope extends LpScopeDef = LpScopeDef> {
  scope: TScope,
  outputPath: LpObjectPath

  receiveFrom(source: ScopeInput<TScope> | NodeOutput<TScope>): void
}

export function isScopeOutput(value: any): value is ScopeOutput {
  return ("scope" satisfies keyof ScopeOutput) in value
    && ("outputPath" satisfies keyof ScopeOutput) in value;
}

// ---------------------------------------------------------------------------------------------------------------------

export type NodeInputPathBuilder<
  TNode extends LpNodeDef
> = (p: ReturnType<typeof typedPath<TNode["_input"]>>) => { $rawPath: Array<string | symbol | number> }

export interface NodeInput<
  TScope extends LpScopeDef = LpScopeDef,
  TNode extends LpNodeDef = LpNodeDef,
> {
  node: TNode,
  inputPath: LpObjectPath

  receiveFrom(source: ScopeInput<TScope> | NodeOutput<TScope>): void
}

export function isNodeInput(value: any): value is NodeInput {
  return ("node" satisfies keyof NodeInput) in value
    && ("inputPath" satisfies keyof NodeInput) in value;
}

// ---------------------------------------------------------------------------------------------------------------------

export type NodeOutputPathBuilder<
  TNode extends LpNodeDef
> = (p: ReturnType<typeof typedPath<TNode["_output"]>>) => { $rawPath: Array<string | symbol | number> };

export interface NodeOutput<
  TScope extends LpScopeDef = LpScopeDef,
  TNode extends LpNodeDef = LpNodeDef
> {
  node: TNode,
  outputPath: LpObjectPath

  sendTo(dest: ScopeOutput<TScope> | NodeInput<TScope>): void
}

export function isNodeOutput(value: any): value is NodeOutput {
  return ("node" satisfies keyof NodeOutput) in value
    && ("outputPath" satisfies keyof NodeOutput) in value;
}

// ---------------------------------------------------------------------------------------------------------------------

export function connect<TScope extends LpScopeDef>(
  source: ScopeInput<TScope> | NodeOutput<TScope>,
  dest: ScopeOutput<TScope> | NodeInput<TScope>,
) {
  const sourceNodeId = isScopeInput(source) ? null : source.node.nodeId
  const sourcePath = isScopeInput(source) ? source.inputPath : source.outputPath

  if (isNodeInput(dest)) {
    const node = dest.node

    node.connections[Object.values(node.connections).length] = {
      objType: "connection_def",
      sourceNodeId,
      sourcePath: Array.from(sourcePath),
      destPath: Array.from(dest.inputPath),
    }
  } else if (isScopeOutput(dest)) {
    dest.scope.connections[Object.values(dest.scope.connections).length] = {
      objType: "connection_def",
      sourceNodeId,
      sourcePath: Array.from(sourcePath),
      destPath: Array.from(dest.outputPath),
    }
  } else {
    throw new Error("Invalid connection")
  }
}
