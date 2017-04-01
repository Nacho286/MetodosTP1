function y = forward_substitution(A,b)
   if size(b,2)~=1 || size(A,1)~=numel(b),
      error('las dimensiones no concuerdan') ;
   end ;
   
   if any(triu(A,1)),
      error('A no es triangular inferior') ;
   end ;
   
   n = size(A,2) ;
   y = zeros(n,1) ;
   
   for i=1:n,
      %y(i) = (b(i) - A(i,1:i-1)*y(1:i-1)) / A(i,i) ;
      %fprintf('%d ', y(i));
      y(i) = b(i);
      for j = 1:i-1
          y(i) = y(i) - A(i,j)*y(j);
      end
      y(i) = y(i)/A(i,i);
   end
end
