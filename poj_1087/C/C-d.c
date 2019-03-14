/* Copyright Derek Kisman (ACM ICPC ECNA 1999) */

int vc[100][400];
int nv1, nv2;

int vm1[100], vm2[400];

int Bipartite() {
        int i, j, k, x, y, z, n;
        int q[400], prev[400], qs, qe;

        n = 0;
        for( i = 0; i < nv1; i++ ) vm1[i] = -1;
        for( i = 0; i < nv2; i++ ) vm2[i] = -1;
        for( i = 0; i < nv1; i++ ) {
                for( j = 0; j < nv2; j++ ) prev[j] = -2;
                qs = qe = 0;
                for( j = 0; j < nv2; j++ ) if( vc[i][j] ) {
                        prev[j] = -1;
                        q[qe++] = j;
                }
                while( qs < qe ) {
                        x = q[qs];
                        if( vm2[x] == -1 ) break;
                        qs++;
                        for( j = 0; j < nv2; j++ ) if( prev[j] == -2 && vc[vm2[x]][j] ) {
                                prev[j] = x;
                                q[qe++] = j;
                        }
                }
                if( qs == qe ) continue;
                while( prev[x] > -1 ) {
                        vm1[vm2[prev[x]]] = x;
                        vm2[x] = vm2[prev[x]];
                        x = prev[x];
                }
                vm2[x] = i;
                vm1[i] = x;
                n++;
        }
        return n;
}

char rec[400][50];
int nrec, rrec;
int rc[400][400];
int drec[100];
int nd;

char buf[1000];

int Find() {
        int i;

        for( i = 0; i < nrec; i++ ) if( !strcmp( buf, rec[i] ) ) break;
        if( i == nrec ) strcpy( rec[nrec++], buf );
        return i;
}

main() {
        int i, j, k, x, y, z, n;

        scanf( " %d", &nrec );
        for( i = 0; i < nrec; i++ ) scanf( " %s", rec[i] );
        rrec = nrec;
        scanf( " %d", &nd );
        for( i = 0; i < nd; i++ ) {
                scanf( " %*s %s", buf );
                drec[i] = Find();
        }
        memset( rc, 0, sizeof(rc) );
        for( i = 0; i < nrec; i++ ) rc[i][i] = 1;
        scanf( " %d", &x );
        for( i = 0; i < x; i++ ) {
                scanf( " %s", buf );
                y = Find();
                scanf( " %s", buf );
                z = Find();
                rc[y][z] = 1;
        }
        for( i = 0; i < nrec; i++ )
                for( j = 0; j < nrec; j++ )
                        for( k = 0; k < nrec; k++ )
                                if( rc[j][i] && rc[i][k] ) rc[j][k] = 1;

        nv1 = nd; nv2 = rrec;
        memset( vc, 0, sizeof(vc) );
        for( i = 0; i < nd; i++ ) for( j = 0; j < rrec; j++ )
                if( rc[drec[i]][j] ) vc[i][j] = 1;
        printf( "%d\n", nv1-Bipartite() );
}


