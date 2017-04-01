function ret = gaussian_elimination( A, b )
%GAUSSIAN_ELIMINATION Gauss without pivoting
   % calcular la cantidad e filas y de columnas de A
   [n,m]=size(A);
   if( n~=m )
      error('La matriz no es cuadrada') ;
   end
   for k = 1:n-1,
      % testear que el coeficiente a dividir no se anule
      if( abs( A(k,k) ) <= eps )
         % podriamos testear si abs( A(i,i) )< 1e-16
         error('Uno de los pivotes es cero. Realizar Gauss con pivoteo parcial') ;
      end
      
      % el corazon del algoritmo
      for i = (k+1):n
         m = A(i,k)./A(k,k);
         A(i,:) = A(i,:)-(m*A(k,:))
        % for j = k:n
        %     A(i,j) = A(i,j)-(m.*A(k,j));
            
        % end
         b(i) = b(i)-(m.*b(k));
      end
   end
   
   % Hacemos backward_substitution.
   ret = backward_substitution(A, b);

end

