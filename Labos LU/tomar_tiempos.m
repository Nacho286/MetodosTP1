function [tiempos_gauss, init_lu, tiempos_lu] = tomar_tiempos(n, cant)
    %{
    Funcion que genera una matriz de tamaño `n` y resuelve `cant` sistemas 
        lineales
    `tiempos_gauss` tiene los tiempos de resolver cada sistema con eliminacion
        gaussiana
    `init_lu` es el tiempo que toma inicializar L y U para la factorizacion LU
    `tiempos_lu` tiene los tiempos de resolver cada sistema con factorizacion
        LU
    %}

    % Me armo una matriz random de nxn tal que tenga solucion
    A = rand(n);

    % Me aseguro que sea simetrica
    A = 0.5*(A + A');

    % Me aseguro que sea definida positiva (pues es edd)
    A = A + eye(n).*double(n);

    % A es sdp, asi que tiene LU y se puede hacer eliminacion gaussiana sin 
    % pivoteo
    
    tic;
    [L,U] = lu_factorization(A);
    init_lu = toc;
    
    tiempos_gauss = zeros(cant,1);
    tiempos_lu = zeros(cant,1);

    for i = 1:cant
        b = rand(n,1);
        
        % Resuelvo con elim gauss
        tic;
        x1 = gaussian_elimination(A, b);
        t = toc;
        tiempos_gauss(i) = t;

        % Resuelvo con lu
        tic;
        y = forward_substitution(L, b);
        x2 = backward_substitution(U, y);
        t = toc;
        tiempos_lu(i) = t;
    end
    tiempos_gauss = tiempos_gauss';
    tiempos_lu = tiempos_lu';
end
