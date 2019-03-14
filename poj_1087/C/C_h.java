/* Copyright Howard Cheng (ACM ICPC ECNA 1999) */

import java.io.*;
import java.util.*;

public class C_h {
  static final int MAX_TYPES = 400;
  static final int MAX_DEVS = 100;
  static final int MAX_ADAPTER = 100;
  static final int MAX_NODES = MAX_TYPES + MAX_DEVS + 2;
  static final int SOURCE = 0;
  static final int SINK = 1;
  static DataInputStream in = new DataInputStream(System.in);
  
  static String type[] = new String[MAX_TYPES];
  static String dev[] = new String[MAX_DEVS];
  static int dev_plug[] = new int[MAX_DEVS];
  static boolean adapter[][] = new boolean[MAX_TYPES][MAX_TYPES];
  static boolean graph[][] = new boolean[MAX_NODES][MAX_NODES];

  static int num_type, n, num_dev;

  static int NODES() {
    return n + num_dev + 2;
  }

  static int PLUG(int x) {
    return x+2;
  }
  
  static int DEV(int x) {
    return x+n+2;
  }

  static int search_type(String s) {
    int i;
    for (i = 0; i < num_type; i++) {
      if (s.equals(type[i])) {
	return i;
      }
    }
    type[num_type++] = new String(s);
    return num_type-1;
  }

  static int read_int() throws IOException {
    String inputline = "";
    inputline = in.readLine();
    StringTokenizer str = new StringTokenizer(inputline);
    return Integer.parseInt(str.nextToken());
  }

  static void read_case() throws IOException {
    String inputline;
    StringTokenizer str;
    int num_adapter, i, j, k;

    n = read_int();
    num_type = n;
    for (i = 0; i < num_type; i++) {
      inputline = in.readLine();
      str = new StringTokenizer(inputline);
      type[i] = new String(str.nextToken());
    }
    num_dev = read_int();
    for (i = 0; i < num_dev; i++) {
      inputline = in.readLine();
      str = new StringTokenizer(inputline);
      dev[i] = new String(str.nextToken());
      dev_plug[i] = search_type(str.nextToken());
    }

    for (i = 0; i < num_type; i++) {
      for (j = 0; j < num_type; j++) {
	adapter[i][j] = (i == j) ? true : false;
      }
    }

    num_adapter = read_int();
    for (i = 0; i < num_adapter; i++) {
      inputline = in.readLine();
      str = new StringTokenizer(inputline);
      j = search_type(str.nextToken());
      k = search_type(str.nextToken());
      adapter[k][j] = true;
    }
    /*
    for (i = 0; i < num_type; i++) {
      System.out.println("Type " + i + ": " + type[i]);
    }
    for (i = 0; i < num_dev; i++) {
      System.out.println("Device " + dev[i] + " requires type " + dev_plug[i]);
    }
    for (i = 0; i < num_type; i++) {
      for (j = 0; j < num_type; j++) {
	if (adapter[i][j] && i != j) {
	  System.out.println("Adapter: " + i + " ==> " + j);
	}
      }
    }
    */
      
  }

  static void trans_closure() {
    int i, j, k;

    for (k = 0; k < num_type; k++) {
      for (i = 0; i < num_type; i++) {
	for (j = 0; j < num_type; j++) {
	  adapter[i][j] |= (adapter[i][k] && adapter[k][j]);
	}
      }
    }
  }

  static void build_graph() {
    int i, j;

    for (i = 0; i < NODES(); i++) {
      for (j = 0; j < NODES(); j++) {
	graph[i][j] = false;
      }
    }

    for (i = 0; i < n; i++) {
      graph[SOURCE][PLUG(i)] = true;
    }
    for (i = 0; i < num_dev; i++) {
      graph[DEV(i)][SINK] = true;
    }

    for (i = 0; i < n; i++) {
      for (j = 0; j < num_dev; j++) {
	graph[PLUG(i)][DEV(j)] = adapter[i][dev_plug[j]];
      }
    }
  }

  static boolean find_path(int v, int pos, int path[], boolean used[]) {
    int w;

    path[pos] = v;
    used[v] = true;
    if (v == SINK) {
      return true;
    }

    for (w = 0; w < NODES(); w++) {
      if (graph[v][w] && !used[w] && find_path(w, pos+1, path, used)) {
	return true;
      }
    }
    return false;
  }

  static void push_path(int path[]) {
    int i;
    for (i = 0; path[i] != SINK; i++) {
      graph[path[i]][path[i+1]] = false;
      graph[path[i+1]][path[i]] = true;
    }
  }

  static void solve_case() {
    int flow = 0;
    int path[] = new int[MAX_NODES];
    int i, j, c;
    boolean used[] = new boolean[MAX_NODES];

    for (i = 0; i < NODES(); i++) {
      used[i] = false;
    }
    while (find_path(SOURCE, 0, path, used)) {
      push_path(path);
      flow++;
      for (i = 0; i < NODES(); i++) {
	used[i] = false;
      }
    }
    System.out.println(num_dev - flow);
  }

  public static void main(String[] args) {
    try {
      read_case();
    } catch (IOException e) {
    }
    trans_closure();
    build_graph();
    solve_case();
  }
}
