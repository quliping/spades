/*
 * graphSimplification.cpp
 *
 *  Created on: 17.03.2011
 */
#include "graphSimplification.hpp"
#include "common.hpp"

bool isPath(Edge &e1, Edge &e2) {
	cerr<<endl<<"("<<e1.FromVertex<<", "<<e1.ToVertex<<") "<<"("<<e2.FromVertex<<", "<<e2.ToVertex<<") ";
	if (e1.ToVertex != e2.FromVertex)
		return false;
	int sts = readLength + insertLength;
	if (e1.length + e2.length + k - 1 <= sts)
		return true;
	int left1 = max(0, e1.length - sts);
	int right1 = min(e1.upper->size(), e1.upper->size() - sts + e2.length);
	int left2 = left1 + sts - e1.length;
	int right2 = right1 + sts - e1.length;
	cerr<<endl<<e1.lower->Subseq(left1, right1).str()<<endl<<e2.upper->Subseq(left2, right2).str();
	return e1.lower->Subseq(left1, right1) == e2.upper->Subseq(left2, right2);
}


bool processLowerSequence(longEdgesMap &longEdges, PairedGraph &graph, int &VertexCount){
	int countIn[MAX_DEGREE], possibleIn[MAX_DEGREE], countOut[MAX_DEGREE], possibleOut[MAX_DEGREE];
	bool res = false;
	forn(curVertId, VertexCount){
		if ((graph.degrees[curVertId][0]!=0)&&(graph.degrees[curVertId][1]!=0)) {

			forn(i,MAX_DEGREE){
				countOut [i] =0;
				countIn [i] =0;
			}
			forn(i,graph.degrees[curVertId][0]){
				int curInEdgeId = edgeRealId(graph.edgeIds[curVertId][i][IN_EDGE], longEdges);
				graph.edgeIds[curVertId][i][IN_EDGE] = curInEdgeId;
				forn(j,graph.degrees[curVertId][1]){
					int curOutEdgeId = edgeRealId(graph.edgeIds[curVertId][j][OUT_EDGE], longEdges);
					graph.edgeIds[curVertId][j][OUT_EDGE] = curOutEdgeId;
					cerr<<"Check isPass for edge "<<curInEdgeId<<" vs "<< curOutEdgeId;
					if (isPath(*longEdges[curInEdgeId], *longEdges[curOutEdgeId]))
					{
						countIn[j]++;
						countOut[i]++;
						possibleIn[j] = i;
						possibleOut[i] =j;
						cerr<<" POSSIBLE"<<endl;
					}
					else cerr<<" IMPOSSIBLE"<<endl;
				}
			}
			forn(i,graph.degrees[curVertId][0]){
				if (countOut[i]==1){
					if (countIn[possibleOut[i]]==1){
						int InEdge = edgeRealId(graph.edgeIds[curVertId][i][IN_EDGE], longEdges);
						int OutEdge = edgeRealId(graph.edgeIds[curVertId][possibleOut[i]][OUT_EDGE], longEdges);
						longEdges[InEdge]->ExpandRight(*longEdges[OutEdge]);
						longEdges[OutEdge] = longEdges[InEdge];
						res = true;
					}
				}
			}
			//REMOVE VERTICES INFO!!! //I did it outside
		}
	}
	return res;
}


/*
bool commonSequencesExtraction(longEdgesMap &longEdges, PairedGraph &graph, int &VertexCount){
	bool res = false;
	forn(curVertexId, VertexCount){


	}
	return res;
}
*/
/* countEdgesDistintion
 * @params:
/* countEdgesDistinction
 * @param vertexId - index of vertex, distances we are interested in
 * @param graph - pairedGraph
 * @returns pair<int,int> - minimal intervals of nucleotids, such that they uniquely represent Inedges and outedges respectively.
 * If one of results  is more than insertLength + 2 * readLength, returns insertLength + 2 * readLength + 1
 *
 */
pair<int, int> vertexDist(longEdgesMap &longEdges, PairedGraph &graph, int vertexId){
	int res1 = k-1;
	int res2 = k-1;
	int max_res = insertLength + 2*readLength + 1;
	//int colors[MAX_DEGREE];
	forn(i, graph.degrees[vertexId][0]) {
		int e1 = graph.edgeIds[vertexId][i][IN_EDGE];
		Sequence tmp = *longEdges[e1]->upper;
		Sequence tmpl = *longEdges[e1]->lower;

//		cerr << endl << "1 " << tmp.str();
		forn(j, i) {
			int count = 0;

			int e2 = graph.edgeIds[vertexId][j][IN_EDGE];
			while (count < max_res) {
				Sequence tmp2 = *longEdges[e2]->upper;
				Sequence tmp2l = *longEdges[e2]->lower;

//				cerr << endl <<"2 "<< tmp2.str();
//				cerr << endl <<count << " ";// << nucl(tmp[tmp.size() - 1 - count]) << " " << nucl (tmp2[tmp2.size() - 1 -count]);
				if (count >= min(tmp.size(), tmp2.size()) || tmp[tmp.size() - 1 - count] != tmp2[tmp2.size() - 1 -count] )
				//				if (*(longEdges[e1]->upper)[count] != *(longEdges[e2]->upper)[count])
					break;
				if (count >= min(tmpl.size(), tmp2l.size()) || tmpl[tmpl.size() - 1 - count] != tmp2l[tmp2l.size() - 1 -count] )
				//				if (*(longEdges[e1]->upper)[count] != *(longEdges[e2]->upper)[count])
					break;

				count ++;
			}
			res1 = max(res1, count);
		}
	}
	forn(i, graph.degrees[vertexId][1]) {
		int e1 = graph.edgeIds[vertexId][i][OUT_EDGE];
		Sequence tmp = *longEdges[e1]->upper;
		Sequence tmpl = *longEdges[e1]->lower;

//		cerr << endl << "1 " << tmp.str();
		forn(j, i) {
			int e2 = graph.edgeIds[vertexId][j][OUT_EDGE];
			int count = 0;
			while (count < max_res) {
				Sequence tmp2 = *longEdges[e2]->upper;
				Sequence tmp2l = *longEdges[e2]->lower;

//				cerr << endl <<"2 "<< tmp2.str();
//				cerr << endl <<count << " ";// << nucl(tmp[tmp.size() - 1 - count]) << " " << nucl (tmp2[tmp2.size() - 1 -count]);
				if (count >= min(tmp.size(), tmp2.size()) || tmp[count] != tmp2[count] )
				//				if (*(longEdges[e1]->upper)[count] != *(longEdges[e2]->upper)[count])
					break;
				if (count >= min(tmpl.size(), tmp2l.size()) || tmpl[count] != tmp2l[count] )
				//				if (*(longEdges[e1]->upper)[count] != *(longEdges[e2]->upper)[count])
					break;

				count ++;
			}
			res2 = max(res2, count);
		}
	}

//	cerr << endl << res1 << " " << res2<<endl;
	//assert(0);
	return make_pair(res1 + 1, res2 + 1);
}



void expandDefinite(longEdgesMap &longEdges, PairedGraph &graph,
		int &VertexCount, bool NotExpandBeyondDefinite) {
	longEdgesMap::iterator it;
	int expandEdgeIndex;
	cerr << "expandDefiniteStart" << endl;
	forn(i,VertexCount) {
		if ((graph.degrees[i][1] == 1) && (graph.degrees[i][0] > 0)) {
			expandEdgeIndex = edgeRealId(graph.edgeIds[i][0][OUT_EDGE], longEdges);
			int DestVertex = longEdges[expandEdgeIndex]->ToVertex;
			pair<int, int> diffDistDest = make_pair(0,0);
			pair<int, int> diffDistCur = make_pair(0,0);
			if (NotExpandBeyondDefinite){
				diffDistDest = vertexDist(longEdges, graph, DestVertex);
				diffDistCur = vertexDist(longEdges, graph, i);
			}
			if	((!NotExpandBeyondDefinite)||(diffDistCur.first+diffDistDest.second+longEdges[expandEdgeIndex]->length<readLength+2*k-1))
			{
				int a = 0;
//				cerr << "trying to expand";
				while ((edgeRealId(graph.edgeIds[DestVertex][a][IN_EDGE], longEdges)
						!= expandEdgeIndex))
					a++;
				assert(a < graph.degrees[DestVertex][0]);
//				cerr << a;
				while (a < graph.degrees[DestVertex][0] - 1) {
					graph.edgeIds[DestVertex][a][IN_EDGE] = graph.edgeIds[DestVertex][a + 1][IN_EDGE];
					a++;
				}

//				cerr << "hm"<<" "<< a << endl << graph.degrees[i][0]<< endl;
				graph.degrees[DestVertex][0]--;
//				assert(graph.degrees[DestVertex][0] > 0);
				forn(j, graph.degrees[i][0]) {
//					cerr << j << endl;
//					cerr <<"index: "<<j << expandEdgeIndex << longEdges[expandEdgeIndex] ->upper ->str() <<endl;
//					cerr.flush();
//					cerr << longEdges[graph.edgeIds[i][j][IN_EDGE]]->upper->str() << endl;
//					//cerr << longEdges[expandEdgeIndex] ->upper ->str() << endl;
//					cerr << endl;
//					cerr <<"index: "<<j << expandEdgeIndex << longEdges[expandEdgeIndex] ->upper ->str() <<endl;

					//cerr << i <<" " << j <<" " << IN_EDGE << graph.edgeIds[i][j][IN_EDGE] << endl;
					longEdges[graph.edgeIds[i][j][IN_EDGE]]->ExpandRight(
							*(longEdges[expandEdgeIndex]));
//					cerr <<"index: "<<j << expandEdgeIndex;
//					cerr.flush();
//					cerr << longEdges[expandEdgeIndex] ->upper ->str() <<endl;
//										cerr.flush();
					graph.edgeIds[DestVertex][graph.degrees[DestVertex][0]][IN_EDGE]
												 = graph.edgeIds[i][j][IN_EDGE];
					graph.degrees[DestVertex][0]++;
//					cerr <<"index: "<<j << expandEdgeIndex << longEdges[expandEdgeIndex] ->upper ->str() <<endl;
//										cerr.flush();

				}
//				cerr << "DOSHLO";
//				cerr.flush();
				it = longEdges.find(expandEdgeIndex);
				longEdges.erase(it);
//				cerr << "expanded";
				graph.degrees[i][0] = 0;
				graph.degrees[i][1] = 0;
			}
		}
	}

	cerr << "expandDefinite second attempt\n" << endl;
	forn(i,VertexCount) {
		if ((graph.degrees[i][0] == 1) && (graph.degrees[i][1] > 0)) {
			cerr << i << endl;
			expandEdgeIndex = edgeRealId(graph.edgeIds[i][0][IN_EDGE], longEdges);
			int SourceVertex = longEdges[expandEdgeIndex]->FromVertex;
			pair<int, int> diffDistSource = make_pair(0,0);
			pair<int, int> diffDistCur = make_pair(0,0);
			if (NotExpandBeyondDefinite){
				diffDistSource = vertexDist(longEdges, graph, SourceVertex);
				diffDistCur = vertexDist(longEdges, graph, i);
			}
			if	((!NotExpandBeyondDefinite)||(diffDistCur.second+diffDistSource.first+longEdges[expandEdgeIndex]->length<readLength+2*k-1))
			{

				int a = 0;
				while (edgeRealId(graph.edgeIds[SourceVertex][a][OUT_EDGE], longEdges)
						!= expandEdgeIndex)
					a++;
				assert(a < graph.degrees[SourceVertex][1]);
				while (a < graph.degrees[SourceVertex][1] - 1) {
					graph.edgeIds[SourceVertex][a][OUT_EDGE]
													= graph.edgeIds[SourceVertex][a + 1][OUT_EDGE];
					a++;
				}
				graph.degrees[SourceVertex][1]--;
				forn(j,graph.degrees[i][1]) {
					longEdges[graph.edgeIds[i][j][OUT_EDGE]]->ExpandLeft(
							*(longEdges[expandEdgeIndex]));
					graph.edgeIds[SourceVertex][graph.degrees[SourceVertex][1]][OUT_EDGE]
													= graph.edgeIds[i][j][OUT_EDGE];
					graph.degrees[SourceVertex][1]++;
				}
				it = longEdges.find(expandEdgeIndex);
				longEdges.erase(it);
				graph.degrees[i][1] = 0;
				graph.degrees[i][0] = 0;

			}
		}
	}
	cerr << "expandDefinite finished\n";
}
