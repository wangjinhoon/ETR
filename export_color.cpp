#if UNITY_EDITOR
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

using IO = System.IO;
using NumberStyles = System.Globalization.NumberStyles;
using CultureInfo = System.Globalization.CultureInfo;

public class ObjFileFormatVertexColorImporter : AssetPostprocessor
{

    Dictionary<string, Color[]> _colorData = new Dictionary<string, Color[]>();

    void OnPreprocessModel()
    {
        if (assetPath.EndsWith(".obj"))
        {
            var stopwatch = System.Diagnostics.Stopwatch.StartNew();

            // read obj data
            List<Color> current_sourceColor = new List<Color>();
            List<int> current_geometryIndices = new List<int>();
            Dictionary<int, int> current_sourceNormalHash = new Dictionary<int, int>();
            Dictionary<int, int> current_sourceUvHash = new Dictionary<int, int>();
            HashSet<Vector3Int> current_geometrySharedVerticesDetector = new HashSet<Vector3Int>();
            Dictionary<string, List<int>> geometryIndices = new Dictionary<string, List<int>>() { { "default", current_geometryIndices } };
            Dictionary<string, List<Color>> sourceColor = new Dictionary<string, List<Color>>() { { "default", current_sourceColor } };
            Dictionary<string, Dictionary<int, int>> sourceNormalHash = new Dictionary<string, Dictionary<int, int>>() { { "default", current_sourceNormalHash } };
            Dictionary<string, Dictionary<int, int>> sourceUvHash = new Dictionary<string, Dictionary<int, int>>() { { "default", current_sourceUvHash } };
            Dictionary<string, HashSet<Vector3Int>> geometrySharedVerticesDetector = new Dictionary<string, HashSet<Vector3Int>>() { { "default", current_geometrySharedVerticesDetector } };
            string line, g = null, o = null, meshName = "default";
            int numVertexColorVertices = 0;
            int v_index = 1, vn_index = 1, vt_index = 1;
            string fileFullPath = Application.dataPath + assetPath.Replace("Assets", "");
            var reader = IO.File.OpenText(fileFullPath);
            while ((line = reader.ReadLine()) != null)
            {
                string[] words = line.Split(' ');

                byte wordsLength = (byte)words.Length;
                byte is_o = words[0] == "o" ? (byte)1 : (byte)0;
                byte is_g = words[0] == "g" ? (byte)1 : (byte)0;
                byte is_v = words[0] == "v" && wordsLength == 7 ? (byte)1 : (byte)0;
                byte is_vt = words[0] == "vt" && wordsLength == 3 ? (byte)1 : (byte)0;
                byte is_vn = words[0] == "vn" && wordsLength == 4 ? (byte)1 : (byte)0;
                byte is_f = words[0] == "f" && wordsLength > 1 ? (byte)1 : (byte)0;

                // parse o
                if (is_o == 1)
                {
                    o = line.Substring(1).Trim(' ').Replace(' ', '_');
                }

                // parse g
                if (is_g == 1)
                {
                    g = line.Substring(1).Trim(' ').Replace(' ', '_');
                    meshName = g;

                    current_sourceColor = new List<Color>();
                    current_geometryIndices = new List<int>();
                    current_sourceNormalHash = new Dictionary<int, int>();
                    current_sourceUvHash = new Dictionary<int, int>();
                    current_geometrySharedVerticesDetector = new HashSet<Vector3Int>();

                    sourceColor.Add(meshName, current_sourceColor);
                    geometryIndices.Add(meshName, current_geometryIndices);
                    sourceNormalHash.Add(meshName, current_sourceNormalHash);
                    sourceUvHash.Add(meshName, current_sourceUvHash);
                    geometrySharedVerticesDetector.Add(meshName, current_geometrySharedVerticesDetector);

                    v_index = 1;
                    vn_index = 1;
                    vt_index = 1;
                }

                // parse face data
                if (is_f == 1)
                {
                    for (int i = 1; i < wordsLength; i++)
                    {
                        string[] v_vt_vn = words[i].Split('/');
                        if (v_vt_vn.Length == 3)
                        {
                            string v = v_vt_vn[0];
                            string vt = v_vt_vn[1];
                            string vn = v_vt_vn[2];

                            int vertexIndex = int.Parse(v, NumberStyles.Number, CultureInfo.InvariantCulture);

                            int normalHash;
                            if (vn.Length != 0)
                            {
                                int normalIndex = int.Parse(vn, NumberStyles.Number, CultureInfo.InvariantCulture);
                                normalHash = current_sourceNormalHash[normalIndex];
                            }
                            else normalHash = -1;// vertex with no normal data


                            int uvHash;
                            if (vt.Length != 0)
                            {
                                int uvIndex = int.Parse(vt, NumberStyles.Number, CultureInfo.InvariantCulture);
                                uvHash = current_sourceUvHash[uvIndex];
                            }
                            else uvHash = -1;// vertex with no uv data

                            if (current_geometrySharedVerticesDetector.Add(new Vector3Int(vertexIndex, uvHash, normalHash)))
                            {
                                //Debug.Log($"new vertex index added: ( {vertexIndex} #{uvHash} , #{normalVectorHash} )");
                                current_geometryIndices.Add(vertexIndex);
                            }
                            //else Debug.Log($"ignoring vertex as shared: ( {vertexIndex} , #{normalVectorHash} )");
                        }
                    }
                }

                // parse vertex data (vertex color only)
                if (is_v == 1)
                {
                    float red = float.Parse(words[4], NumberStyles.Number, CultureInfo.InvariantCulture);
                    float green = float.Parse(words[5], NumberStyles.Number, CultureInfo.InvariantCulture);
                    float blue = float.Parse(words[6], NumberStyles.Number, CultureInfo.InvariantCulture);
                    current_sourceColor.Add(new Color(red, green, blue));
                    v_index++;
                    numVertexColorVertices++;
                }

                // parse vertex normal data
                if (is_vn == 1)
                {
                    int hash = 17;
                    unchecked
                    {
                        hash = hash * 23 + words[1].GetHashCode();
                        hash = hash * 23 + words[2].GetHashCode();
                        hash = hash * 23 + words[3].GetHashCode();
                    }
                    //Debug.Log($"vn #{vn_index} ( {words[1]} , {words[2]} , {words[3]} ) added as #{hash}");
                    current_sourceNormalHash.Add(vn_index++, hash);
                }

                // parse texture UV data
                if (is_vt == 1)
                {
                    int hash = 17;
                    unchecked
                    {
                        hash = hash * 23 + words[1].GetHashCode();
                        hash = hash * 23 + words[2].GetHashCode();
                    }
                    //Debug.Log($"vt #{vn_index} ( {words[1]} , {words[2]} ) added as #{hash}");
                    current_sourceUvHash.Add(vt_index++, hash);
                }
            }
            reader.Dispose();

            if (numVertexColorVertices != 0)// was any vertex color data found ?
            {
                // change importer settings
                var importer = (ModelImporter)assetImporter;
                importer.optimizeMeshVertices = false;
                importer.optimizeMeshPolygons = false;
                importer.weldVertices = false;
                importer.importNormals = ModelImporterNormals.Import;
                importer.importTangents = ModelImporterTangents.CalculateMikk;
                importer.importAnimation = false;
                importer.animationType = ModelImporterAnimationType.None;
                importer.materialImportMode = ModelImporterMaterialImportMode.None;

                // prepare final color array
                foreach (string nextMeshName in sourceColor.Keys)
                {
                    List<int> nextMeshIndices = geometryIndices[nextMeshName];
                    int numIndices = nextMeshIndices.Count;

                    List<Color> unpreparedColorArray = sourceColor[nextMeshName];
                    Color[] finalColorArray = new Color[numIndices];

                    for (int i = 0; i < numIndices; i++)
                    {
                        finalColorArray[i] = unpreparedColorArray[nextMeshIndices[i] - 1];
                    }

                    _colorData.Add(nextMeshName, finalColorArray);
                }
            }

            double totalSeconds = new System.TimeSpan(stopwatch.ElapsedTicks).TotalSeconds;
            if (totalSeconds > 0.1) Debug.LogWarning($"{GetType().Name}::{nameof(OnPreprocessModel)}() took {totalSeconds:0.00} seconds, `{assetPath}` asset");
        }
    }

    void OnPostprocessModel(GameObject gameObject)
    {
        if (!assetPath.EndsWith(".obj")) return;

        var stopwatch = System.Diagnostics.Stopwatch.StartNew();

        if (_colorData.Count != 0)
        {
            MeshFilter[] meshFilters = gameObject.GetComponentsInChildren<MeshFilter>();
            //Debug.Log($"meshFilters:{meshFilters.Length}, _colorData.Count:{_colorData.Count}",gameObject);
            foreach (var mf in meshFilters)
            {
                Mesh mesh = mf.sharedMesh;
                var vertexColor = _colorData[mesh.name];
                if (vertexColor.Length != mesh.vertexCount) Debug.LogError($"Invalid color data length! mesh.name:'{mesh.name}', vertexColor.Length:{vertexColor.Length}, mesh.vertexCount:{mesh.vertexCount}", gameObject);
                mesh.SetColors(vertexColor);
            }
        }

        double totalSeconds = new System.TimeSpan(stopwatch.ElapsedTicks).TotalSeconds;
        if (totalSeconds > 0.1) Debug.LogWarning($"{GetType().Name}::{nameof(OnPostprocessModel)}() took {totalSeconds:0.00} seconds, `{assetPath}` asset", gameObject);
    }

}
#endif
