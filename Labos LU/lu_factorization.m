function [L, U] = lu_factorization(A)
    n = size(A);
    L = eye(n);
    U = A;

    for k = 1:(n-1)
        if U(k,k) == 0
            error('La matriz no tiene factorizacion LU. Usar factorizacion PLU.')
        end
        for i = k+1:n
            m = U(i,k)/U(k,k);
            U(i,:) = U(i,:) - m * U(k,:);
            L(i,k) = m;
        end
    end

    if U(n,n) == 0
        error('El sistema es indeterminado.')
    end

end
